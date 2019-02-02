//
// Created by joao on 11/12/17.
//

#include <iostream>
#include "Cameras/OpenVRCamera.h"
#include <openvr.h>
#include "glm_wrapper.h"

#ifdef OPENVR

OpenVRCamera::OpenVRCamera(Vec3 originalPosition, Quat originalOrientation) {
    _position = originalPosition;
    _orientation = originalOrientation;
    calculateBillboard();

    if(!vr::VR_IsRuntimeInstalled() || !vr::VR_IsHmdPresent()){
        std::cout << "VR NOT READY!" << std::endl;
        _VRReady = false;
        return;
    }

    _VRReady = true;
    vr::EVRInitError initError;
    char* initInfo = nullptr;
    _hmd = vr::VR_Init(&initError, vr::VRApplication_Scene, initInfo);
    if(!_hmd){
        std::cerr << vr::VR_GetVRInitErrorAsEnglishDescription(initError) << std::endl;
        _VRReady = false;
        return;
    }
    std::cout << initInfo << std::endl;
    if(!vr::VRCompositor()){
        std::cerr << "Failed to initialize VR compositor!" << std::endl;
        _VRReady = false;
        return;
    }
    _VRReady = true;
    _currentEye = vr::Eye_Left;
    _currentEyeIndex = LEFT_INDEX;

    _hmd->GetRecommendedRenderTargetSize( &_recommendedWidth, &_recommendedHeight);

    //vr::VRCompositor()->CompositorBringToFront();
    _correction = Quat(1.0f, 0.0f, 0.0f, 0.0f);

}

void OpenVRCamera::perspective(float near, float far) {
    vr::HmdMatrix44_t mat = _hmd->GetProjectionMatrix(vr::Eye_Left , near, far);
    _eyeProjection[LEFT_INDEX] = Mat4
            (
                mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
                mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
                mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
                mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
            );
    _eyeInverseProjection[LEFT_INDEX] = glm::inverse(_eyeProjection[LEFT_INDEX]);

    mat = _hmd->GetProjectionMatrix(vr::Eye_Right , near, far);
    _eyeProjection[RIGHT_INDEX] = Mat4
            (
                mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
                mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
                mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
                mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
            );
    _eyeInverseProjection[RIGHT_INDEX] = glm::inverse(_eyeProjection[RIGHT_INDEX]);


    vr::HmdMatrix34_t matEye = _hmd->GetEyeToHeadTransform( vr::Eye_Left );
    _eyeTransform[LEFT_INDEX] = glm::inverse(Mat4
            (
                matEye.m[0][0], matEye.m[1][0], matEye.m[2][0], 0.0,
                matEye.m[0][1], matEye.m[1][1], matEye.m[2][1], 0.0,
                matEye.m[0][2], matEye.m[1][2], matEye.m[2][2], 0.0,
                matEye.m[0][3], matEye.m[1][3], matEye.m[2][3], 1.0f
            ));
    matEye = _hmd->GetEyeToHeadTransform( vr::Eye_Right );
    _eyeTransform[RIGHT_INDEX] = glm::inverse(Mat4
            (
                matEye.m[0][0], matEye.m[1][0], matEye.m[2][0], 0.0,
                matEye.m[0][1], matEye.m[1][1], matEye.m[2][1], 0.0,
                matEye.m[0][2], matEye.m[1][2], matEye.m[2][2], 0.0,
                matEye.m[0][3], matEye.m[1][3], matEye.m[2][3], 1.0f
            ));


}

void OpenVRCamera::updatePose() {
    vr::VRCompositor()->WaitGetPoses(_trackedDevicePose, vr::k_unMaxTrackedDeviceCount, nullptr, 0 );

    for(uint32_t nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice ){
        if (_trackedDevicePose[nDevice].bPoseIsValid){
            if(_hmd->GetTrackedDeviceClass(nDevice) == vr::TrackedDeviceClass_HMD) {

                vr::HmdMatrix34_t mat = _trackedDevicePose[nDevice].mDeviceToAbsoluteTracking;
                _devicePose[nDevice] = Mat4(
                            mat.m[0][0], mat.m[1][0], mat.m[2][0], 0.0,
                            mat.m[0][1], mat.m[1][1], mat.m[2][1], 0.0,
                            mat.m[0][2], mat.m[1][2], mat.m[2][2], 0.0,
                            mat.m[0][3], mat.m[1][3], mat.m[2][3], 1.0f
                        );
            }
        }
    }

    if (_trackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid )
    {
        _rawOrientation = glm::toQuat(glm::inverse(_devicePose[vr::k_unTrackedDeviceIndex_Hmd]));
        _orientation = _correction * _rawOrientation;
    }
}

void OpenVRCamera::recenter() {
    _correction = glm::inverse(_rawOrientation);
}

void OpenVRCamera::submit(ColorTextureFrameBuffer *leftFBO, ColorTextureFrameBuffer *rightFBO) {
    GLuint textureId = leftFBO->getTexture()->getTexture();
    vr::Texture_t leftEyeTexture = {(void*)(uintptr_t)textureId, vr::TextureType_OpenGL, vr::ColorSpace_Linear};
    vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture );

    textureId = rightFBO->getTexture()->getTexture();
    vr::Texture_t rightEyeTexture = {(void*)(uintptr_t)textureId, vr::TextureType_OpenGL, vr::ColorSpace_Linear};
    vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);
}


int OpenVRCamera::getRecommendedHeight() {
    return (int)_recommendedHeight;
}

int OpenVRCamera::getRecommendedWidth() {
    return (int)_recommendedWidth;
}

Mat4 OpenVRCamera::getMatrix() {
    return _projection * produceViewMatrix();
}

Mat4 OpenVRCamera::produceViewMatrix() {
    return _eyeTransform[_currentEyeIndex] * glm::toMat4(_orientation) * glm::translate(Mat4(1.0f), Vec3(-_position[0], -_position[1], -_position[2]));
}

Mat4 OpenVRCamera::getInverseViewMatrix() {
    return glm::inverse(getViewMatrix());
    /*return glm::translate(Mat4(1.0f), Vec3(_position[0], _position[1], _position[2]))*
            glm::toMat4(glm::inverse(_orientation));*/
}

void OpenVRCamera::move(float x, float y, float z){
    _position += Vec3(x, y, z) * _orientation;
}
void OpenVRCamera::changeOrientation(float yaw, float pitch, float roll){
    glm::quat q = glm::quat(glm::vec3(pitch, yaw, roll));
    _orientation = q * _orientation;
    glm::normalize(_orientation);

    /*_orientation = glm::angleAxis(yaw, up) * _orientation;
    _orientation = glm::angleAxis(pitch, right) * _orientation;
    _orientation = glm::angleAxis(roll, front) * _orientation;
    glm::normalize(_orientation);*/


    calculateBillboard();
}


void OpenVRCamera::resize(int x, int y) {
    //do nothing
    return;
    //Camera::resize(x, y);
}

bool OpenVRCamera::isReady() {
    return _VRReady;
}

void OpenVRCamera::setCurrentEye(int eye) {
    _currentEye = (eye == EYE_LEFT)?vr::Eye_Left:vr::Eye_Right;
    if(eye == EYE_LEFT)
        _currentEyeIndex = LEFT_INDEX;
    else
        _currentEyeIndex = RIGHT_INDEX;

    _projection = _eyeProjection[_currentEyeIndex];
    _inverseProjection = _eyeInverseProjection[_currentEyeIndex];
}

OpenVRCamera::~OpenVRCamera(){
    if(_VRReady){
        vr::VR_Shutdown();
    }
}

#endif //OPENVR