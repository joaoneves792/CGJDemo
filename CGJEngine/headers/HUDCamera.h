//
// Created by joao on 11/12/17.
//

#ifndef CGJM_HUDCAMERA_H
#define CGJM_HUDCAMERA_H

#include "Camera.h"

class HUDCamera : public Camera{
public:
    HUDCamera(float left, float right, float top, float bottom, float near, float far);
    virtual Mat4 getMatrix();
    virtual Mat4 getViewMatrix();
    virtual void resize(int x, int y);
    virtual void move(float x, float y, float z);
    virtual void changeOrientation(float yaw, float pitch, float roll);
    virtual ~HUDCamera();
};
#endif //CGJM_HUDCAMERA_H
