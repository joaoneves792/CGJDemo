//
// Created by joao on 11/12/17.
//

#ifndef CGJM_CAMERA_H
#define CGJM_CAMERA_H

#include "quat.h"
#include "mat.h"
#include "vec.h"

const Vec3 up(0.0f, 1.0f, 0.0f);
const Vec3 front(0.0f, 0.0f, -1.0f);
const Vec3 right(1.0f, 0.0f, 0.0f);

class Camera{
protected:
    Vec3 position;
    Quat orientation;

    Mat4 projection;
public:
    virtual Mat4 getMatrix()=0;
    Mat4 getProjectionMatrix();
    virtual Mat4 getViewMatrix()=0;
    virtual void move(float x, float y, float z)=0;
    virtual void changeOrientation(float yaw, float pitch, float roll)=0;
    void perspective(float fovy, float aspectRatio, float near, float far);
    void ortho(float left, float right, float top, float bottom, float near, float far);
    virtual ~Camera()= default;
};

#endif //CGJM_CAMERA_H
