//
// Created by joao on 11/12/17.
//

#ifndef CGJM_OPENHMDCAMERA_H
#define CGJM_OPENHMDCAMERA_H

#ifdef OPENHMD

#include "Cameras/VRCamera.h"
#include "FBOs/ColorTextureFrameBuffer.h"
#include "glm_wrapper.h"
#include <openhmd.h>
#include <Shader.h>
#include <Meshes/QuadMesh.h>

#define LEFT_INDEX 0
#define RIGHT_INDEX 1


class OpenHMDCamera : public VRCamera{
private:
    bool _VRReady;
    Mat4 _eyeProjection[2];
    Mat4 _eyeInverseProjection[2];
    Mat4 _eyeTransform[2];
    Quat _rawOrientation;
    Quat _correction;
    ohmd_context* _ctx;
    ohmd_device* _hmd;
    int _currentEye;
    int _currentEyeIndex;
    int _recommendedWidth;
    int _recommendedHeight;

    Mat4 _screenMVP[2];

    float _ipd;
    float _viewport_scale[2];
    float _distortion_coeffs[4];
    float _aberr_scale[3];
    float _sep;
    float _left_lens_center[2];
    float _right_lens_center[2];
    float _warpScale;
    float _warpAdj;
    Shader* _shader;
    QuadMesh* _quad;

    const char* hmd_vertex_glsl =
            "#version 330\n"
            "layout (location=0) in vec3 position;\n"
            "uniform mat4 mvp;\n"
            "out vec2 T;\n"
            "void main(void){\n"
            "    T = (position.xy+1)*0.5;\n"
            "    gl_Position = mvp * vec4(position, 1.0);\n"
            "}";

public:
    OpenHMDCamera(Vec3 originalPosition, Quat originalOrientation);
    virtual Mat4 getMatrix();
    virtual Mat4 produceViewMatrix();
    virtual Mat4 getInverseViewMatrix();
    virtual void move(float x, float y, float z);
    virtual void changeOrientation(float yaw, float pitch, float roll);
    void perspective(float near, float far);
    virtual void resize(int x, int y);
    void updatePose();
    bool isReady();
    void setCurrentEye(int eye);
    int getRecommendedWidth();
    int getRecommendedHeight();
    void recenter();
    void submit(ColorTextureFrameBuffer* leftFBO, ColorTextureFrameBuffer* rightFBO);

    virtual ~OpenHMDCamera();
};
#endif //OPENHMD
#endif //CGJM_OPENHMDCAMERA_H
