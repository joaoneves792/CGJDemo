//
// Created by joao on 11/12/17.
//

#ifndef CGJM_VRCAMERA_H
#define CGJM_VRCAMERA_H

#include "Cameras/Camera.h"
#include "FBOs/ColorTextureFrameBuffer.h"
#include "glm_wrapper.h"
#include <openvr.h>

#define EYE_LEFT vr::Eye_Left
#define EYE_RIGHT vr::Eye_Right

#define LEFT_INDEX 0
#define RIGHT_INDEX 1

class VRCamera : public Camera{
private:
    bool _VRReady;
    Mat4 _eyeProjection[2];
    Mat4 _eyeInverseProjection[2];
    Mat4 _eyeTransform[2];
    Quat _correction;
    vr::IVRSystem* _hmd;
    vr::TrackedDevicePose_t _trackedDevicePose[ vr::k_unMaxTrackedDeviceCount ];
    Mat4 _devicePose[ vr::k_unMaxTrackedDeviceCount ];
    vr::EVREye _currentEye;
    int _currentEyeIndex;
    uint32_t _recommendedWidth;
    uint32_t _recommendedHeight;

public:
    VRCamera(Vec3 originalPosition, Quat originalOrientation);
    virtual Mat4 getMatrix();
    virtual Mat4 produceViewMatrix();
    virtual Mat4 getInverseViewMatrix();
    virtual void move(float x, float y, float z);
    virtual void changeOrientation(float yaw, float pitch, float roll);
    void perspective(float near, float far);
    virtual void resize(int x, int y);
    void updatePose();
    bool isReady();
    void setCurrentEye(vr::EVREye eye);
    int getRecommendedWidth();
    int getRecommendedHeight();
    void recenter();
    void submit(ColorTextureFrameBuffer* leftFBO, ColorTextureFrameBuffer* rightFBO);

    virtual ~VRCamera();
};
#endif //CGJM_VRCAMERA_H
