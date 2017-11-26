//
// Created by joao on 11/12/17.
//

#ifndef CGJM_FREECAMERA_H
#define CGJM_FREECAMERA_H

#include "Camera.h"
#include "glm_wrapper.h"

class FreeCamera : public Camera{
public:
    FreeCamera(Vec3 originalPosition, Quat originalOrientation);
    virtual Mat4 getMatrix();
    virtual Mat4 getViewMatrix();
    virtual void move(float x, float y, float z);
    virtual void changeOrientation(float yaw, float pitch, float roll);
    virtual void resize(int x, int y);
    virtual ~FreeCamera();
};
#endif //CGJM_FREECAMERA_H
