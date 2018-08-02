//
// Created by joao on 11/12/17.
//

#ifndef CGJM_SPHERECAMERA_H
#define CGJM_SPHERECAMERA_H

#include "Cameras/Camera.h"
#include "glm_wrapper.h"

class SphereCamera : public Camera{
    float _distance;
    Vec3 _center;
public:
    SphereCamera(float distance, Vec3 center, Quat originalOrientation);
    virtual Mat4 getMatrix();
    virtual Mat4 produceViewMatrix();
    virtual Mat4 getInverseViewMatrix();
    virtual void move(float x, float y, float z);
    virtual void changeOrientation(float yaw, float pitch, float roll);
    virtual void resize(int x, int y);
    virtual ~SphereCamera();
};
#endif //CGJM_SPHERECAMERA_H
