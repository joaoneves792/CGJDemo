//
// Created by joao on 11/12/17.
//

#ifndef CGJM_SPHERECAMERA_H
#define CGJM_SPHERECAMERA_H

#include "Camera.h"

class SphereCamera : public Camera{
    float distance;
public:
    SphereCamera(float distance, Vec3 center, Quat originalOrientation);
    virtual Mat4 getMatrix();
    virtual Mat4 getViewMatrix();
    virtual void move(float x, float y, float z);
    virtual void changeOrientation(float yaw, float pitch, float roll);
    virtual ~SphereCamera();
};
#endif //CGJM_SPHERECAMERA_H
