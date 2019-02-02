//
// Created by joao on 01/02/19.
//

#include <iostream>
#include "Cameras/OpenHMDCamera.h"
#include <openhmd.h>
#include <Meshes/QuadMesh.h>
#include "glm_wrapper.h"

#ifdef OPENHMD

OpenHMDCamera::OpenHMDCamera(Vec3 originalPosition, Quat originalOrientation) {
    _position = originalPosition;
    _orientation = originalOrientation;
    calculateBillboard();

    _ctx = ohmd_ctx_create();
    int num_devices = ohmd_ctx_probe(_ctx);
    if(num_devices < 0){
        std::cout << "failed to probe devices: " <<  ohmd_ctx_get_error(_ctx) << std::endl;
        _VRReady = false;
        return;
    }

    ohmd_device_settings* settings = ohmd_device_settings_create(_ctx);

    // If OHMD_IDS_AUTOMATIC_UPDATE is set to 0, ohmd_ctx_update() must be called at least 10 times per second.
    // It is enabled by default.
    int auto_update = 1;
    ohmd_device_settings_seti(settings, OHMD_IDS_AUTOMATIC_UPDATE, &auto_update);

    _hmd = ohmd_list_open_device_s(_ctx, 0, settings);
    if(!_hmd){
        std::cout << "failed to open device: " << ohmd_ctx_get_error(_ctx) << std::endl;
        _VRReady = false;
        return;
    }

    _VRReady = true;
    _currentEye = EYE_LEFT;
    _currentEyeIndex = LEFT_INDEX;

    ohmd_device_geti(_hmd, OHMD_SCREEN_HORIZONTAL_RESOLUTION, &_recommendedWidth);
    _recommendedWidth /= 2;
    ohmd_device_geti(_hmd, OHMD_SCREEN_VERTICAL_RESOLUTION, &_recommendedHeight);
    float ipd;//TODO Figure out what this is
    ohmd_device_getf(_hmd, OHMD_EYE_IPD, &ipd);


    //viewport is half the screen
    ohmd_device_getf(_hmd, OHMD_SCREEN_HORIZONTAL_SIZE, &(_viewport_scale[0]));
    _viewport_scale[0] /= 2.0f;
    ohmd_device_getf(_hmd, OHMD_SCREEN_VERTICAL_SIZE, &(_viewport_scale[1]));
    //distortion coefficients
    ohmd_device_getf(_hmd, OHMD_UNIVERSAL_DISTORTION_K, &(_distortion_coeffs[0]));
    ohmd_device_getf(_hmd, OHMD_UNIVERSAL_ABERRATION_K, &(_aberr_scale[0]));
    //calculate lens centers (assuming the eye separation is the distance between the lens centers)
    ohmd_device_getf(_hmd, OHMD_LENS_HORIZONTAL_SEPARATION, &_sep);
    ohmd_device_getf(_hmd, OHMD_LENS_VERTICAL_POSITION, &(_left_lens_center[1]));
    ohmd_device_getf(_hmd, OHMD_LENS_VERTICAL_POSITION, &(_right_lens_center[1]));
    _left_lens_center[0] = _viewport_scale[0] - _sep/2.0f;
    _right_lens_center[0] = _sep/2.0f;
    //assume calibration was for lens view to which ever edge of screen is further away from lens center
    _warpScale = (_left_lens_center[0] > _right_lens_center[0]) ? _left_lens_center[0] : _right_lens_center[0];
    _warpAdj = 1.0f;

    ohmd_device_settings_destroy(settings);

    _correction = Quat(1.0f, 0.0f, 0.0f, 0.0f);


    const char* vertex;
    ohmd_gets(OHMD_GLSL_330_DISTORTION_VERT_SRC, &vertex);
    const char* fragment;
    ohmd_gets(OHMD_GLSL_330_DISTORTION_FRAG_SRC, &fragment);

    _shader = new Shader();
    _shader->loadFromString(hmd_vertex_glsl, fragment);
    _shader->link();
    _shader->use();
    glUniform1i(glGetUniformLocation(_shader->getShader(), "warpTexture"), 0);
    glUniform1f(glGetUniformLocation(_shader->getShader(), "WarpScale"), _warpScale*_warpAdj);
    glUniform2fv(glGetUniformLocation(_shader->getShader(), "ViewportScale"), 1, _viewport_scale);
    glUniform3fv(glGetUniformLocation(_shader->getShader(), "aberr"), 1, _aberr_scale);
    glUniform4fv(glGetUniformLocation(_shader->getShader(), "HmdWarpParam"), 1, _distortion_coeffs);
    glUniformMatrix4fv(glGetUniformLocation(_shader->getShader(), "mvp"), 1, GL_FALSE, glm::value_ptr(Mat4(1.0f)));

    _quad = new QuadMesh();

    Mat4 scale = glm::scale(Mat4(1.0f), Vec3(0.5f, 1.0f, 1.0f));
    _screenMVP[LEFT_INDEX] = glm::translate(Mat4(1.0f), Vec3(-0.5f, 0.0f, 0.0f)) * scale;
    _screenMVP[RIGHT_INDEX] = glm::translate(Mat4(1.0f), Vec3(+0.5f, 0.0f, 0.0f))* scale;
}

void OpenHMDCamera::perspective(float near, float far) {
    float mat[16];
    ohmd_device_getf(_hmd, OHMD_LEFT_EYE_GL_PROJECTION_MATRIX, mat);
    _eyeProjection[LEFT_INDEX] = Mat4(
            mat[0],  mat[1],  mat[2],  mat[3],
            mat[4],  mat[5],  mat[6],  mat[7],
            mat[8],  mat[9],  mat[10], mat[11],
            mat[12], mat[13], mat[14], mat[15]
            );
    _eyeInverseProjection[LEFT_INDEX] = glm::inverse(_eyeProjection[LEFT_INDEX]);

    ohmd_device_getf(_hmd, OHMD_RIGHT_EYE_GL_PROJECTION_MATRIX, mat);
    _eyeProjection[RIGHT_INDEX] = Mat4(
            mat[0],  mat[1],  mat[2],  mat[3],
            mat[4],  mat[5],  mat[6],  mat[7],
            mat[8],  mat[9],  mat[10], mat[11],
            mat[12], mat[13], mat[14], mat[15]
    );
    _eyeInverseProjection[RIGHT_INDEX] = glm::inverse(_eyeProjection[RIGHT_INDEX]);

    _eyeTransform[LEFT_INDEX] = glm::translate(Mat4(1.0f), Vec3(-(_sep/2.0f), 0.0f, 0.0f));
    _eyeTransform[RIGHT_INDEX] = glm::translate(Mat4(1.0f), Vec3((_sep/2.0f), 0.0f, 0.0f));
}

void OpenHMDCamera::updatePose() {
    ohmd_ctx_update(_ctx);
    float rotation[4];
    ohmd_device_getf(_hmd, OHMD_ROTATION_QUAT, rotation);
    _rawOrientation = glm::inverse(Quat(rotation[3], rotation[0], rotation[1], rotation[2]));
    _orientation = _correction * _rawOrientation;
}

void OpenHMDCamera::recenter() {
    _correction = glm::inverse(_rawOrientation);
}

void OpenHMDCamera::submit(ColorTextureFrameBuffer *leftFBO, ColorTextureFrameBuffer *rightFBO) {
    _shader->use();

    static GLint lensCenterLoc = glGetUniformLocation(_shader->getShader(), "LensCenter");
    static GLint MVPLoc = glGetUniformLocation(_shader->getShader(), "mvp");

    glViewport(0, 0, _recommendedWidth*2, _recommendedHeight);

    glUniform2fv(lensCenterLoc, 1, _left_lens_center);
    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(_screenMVP[LEFT_INDEX]));
    glActiveTexture(GL_TEXTURE0);
    leftFBO->bindTexture();
    _quad->draw();

    glUniform2fv(lensCenterLoc, 1, _right_lens_center);
    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(_screenMVP[RIGHT_INDEX]));
    rightFBO->bindTexture();
    _quad->draw();
}


int OpenHMDCamera::getRecommendedHeight() {
    return _recommendedHeight;
}

int OpenHMDCamera::getRecommendedWidth() {
    return _recommendedWidth;
}

Mat4 OpenHMDCamera::getMatrix() {
    return _projection * produceViewMatrix();
}

Mat4 OpenHMDCamera::produceViewMatrix() {
    return _eyeTransform[_currentEyeIndex] * glm::toMat4(_orientation) * glm::translate(Mat4(1.0f), Vec3(-_position[0], -_position[1], -_position[2]));
}

Mat4 OpenHMDCamera::getInverseViewMatrix() {
    return glm::inverse(getViewMatrix());
    /*return glm::translate(Mat4(1.0f), Vec3(_position[0], _position[1], _position[2]))*
            glm::toMat4(glm::inverse(_orientation));*/
}

void OpenHMDCamera::move(float x, float y, float z){
    _position += Vec3(x, y, z) * _orientation;
}

void OpenHMDCamera::changeOrientation(float yaw, float pitch, float roll){
    glm::quat q = glm::quat(glm::vec3(pitch, yaw, roll));
    _orientation = q * _orientation;
    glm::normalize(_orientation);
    calculateBillboard();
}


void OpenHMDCamera::resize(int x, int y) {
    //do nothing
    return;
    //Camera::resize(x, y);
}

bool OpenHMDCamera::isReady() {
    return _VRReady;
}

void OpenHMDCamera::setCurrentEye(int eye) {
    _currentEye = eye;
    if(eye == EYE_LEFT) {
        _currentEyeIndex = LEFT_INDEX;
        _shader->use();
        glUniform2fv(glGetUniformLocation(_shader->getShader(), "LensCenter"), 1, _left_lens_center);
    }else {
        _currentEyeIndex = RIGHT_INDEX;
        _shader->use();
        glUniform2fv(glGetUniformLocation(_shader->getShader(), "LensCenter"), 1, _right_lens_center);
    }

    _projection = _eyeProjection[_currentEyeIndex];
    _inverseProjection = _eyeInverseProjection[_currentEyeIndex];
}

OpenHMDCamera::~OpenHMDCamera(){
    if(_VRReady){
        ohmd_ctx_destroy(_ctx);
        delete _shader;
        delete _quad;
    }
}

#endif //OPENVR