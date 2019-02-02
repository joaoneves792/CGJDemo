//
// Created by joao on 11/12/17.
//

#ifndef CGJM_OPENVRCAMERA_H
#define CGJM_OPENVRCAMERA_H

#ifdef OPENVR

#include "Cameras/VRCamera.h"
#include "FBOs/ColorTextureFrameBuffer.h"
#include "glm_wrapper.h"
#include <openvr.h>

#define LEFT_INDEX 0
#define RIGHT_INDEX 1


class OpenVRCamera : public VRCamera{
private:
    bool _VRReady;
    Mat4 _eyeProjection[2];
    Mat4 _eyeInverseProjection[2];
    Mat4 _eyeTransform[2];
    Quat _rawOrientation;
    Quat _correction;
    vr::IVRSystem* _hmd;
    vr::TrackedDevicePose_t _trackedDevicePose[ vr::k_unMaxTrackedDeviceCount ];
    Mat4 _devicePose[ vr::k_unMaxTrackedDeviceCount ];
    vr::EVREye _currentEye;
    int _currentEyeIndex;
    uint32_t _recommendedWidth;
    uint32_t _recommendedHeight;

public:
    OpenVRCamera(Vec3 originalPosition, Quat originalOrientation);
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

    virtual ~OpenVRCamera();
};
#endif //OPENVR
#endif //CGJM_OPENVRCAMERA_H
