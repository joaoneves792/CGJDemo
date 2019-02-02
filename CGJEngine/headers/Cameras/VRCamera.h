//
// Created by joao on 11/12/17.
//

#ifndef CGJM_VRCAMERA_H
#define CGJM_VRCAMERA_H

#include "Cameras/Camera.h"
#include "FBOs/ColorTextureFrameBuffer.h"
#include "glm_wrapper.h"


#define EYE_LEFT 1
#define EYE_RIGHT 2

#define LEFT_INDEX 0
#define RIGHT_INDEX 1


class VRCamera : public Camera{
public:
    virtual Mat4 getMatrix() = 0;
    virtual Mat4 produceViewMatrix() = 0;
    virtual Mat4 getInverseViewMatrix() = 0;
    virtual void move(float x, float y, float z) = 0;
    virtual void changeOrientation(float yaw, float pitch, float roll) = 0;
    virtual void perspective(float near, float far) = 0;
    virtual void resize(int x, int y) = 0;
    virtual void updatePose() = 0;
    virtual bool isReady() = 0;
    virtual void setCurrentEye(int eye) = 0;
    virtual int getRecommendedWidth() = 0;
    virtual int getRecommendedHeight() = 0;
    virtual void recenter() = 0;
    virtual void submit(ColorTextureFrameBuffer* leftFBO, ColorTextureFrameBuffer* rightFBO) = 0;

};
#endif //CGJM_VRCAMERA_H
