//
// Created by joao on 11/12/17.
//

#ifndef CGJM_HUDCAMERA_H
#define CGJM_HUDCAMERA_H

#include "Cameras/Camera.h"
#include "glm_wrapper.h"

class HUDCamera : public Camera{
private:
    float _left, _right, _top, _bottom;
    bool _scale;
public:
    HUDCamera(float left, float right, float top, float bottom, float near, float far, bool scale);
    virtual Mat4 getMatrix();
    virtual Mat4 produceViewMatrix();
    virtual Mat4 getInverseViewMatrix();
    virtual void resize(int x, int y);
    virtual void move(float x, float y, float z);
    virtual void changeOrientation(float yaw, float pitch, float roll);
    virtual ~HUDCamera();
};
#endif //CGJM_HUDCAMERA_H
