//
// Created by joao on 11/12/17.
//

#ifndef CGJM_CAMERA_H
#define CGJM_CAMERA_H

#include "glm_wrapper.h"

const Vec3 up(0.0f, 1.0f, 0.0f);
const Vec3 front(0.0f, 0.0f, -1.0f);
const Vec3 right(1.0f, 0.0f, 0.0f);

class Camera{
protected:
    Vec3 _position;
    Quat _orientation;

    Quat _pseudoBillboard;

    Mat4 _projection;
    float _fovy;
    float _near;
    float _far;

protected:
    void calculateBillboard();
public:
    virtual Mat4 getMatrix()=0;
    Mat4 getProjectionMatrix();
    Quat getBillboardOrientation();
    virtual Mat4 getViewMatrix()=0;
    virtual void move(float x, float y, float z)=0;
    virtual void changeOrientation(float yaw, float pitch, float roll)=0;
    virtual void resize(int x, int y);
    void perspective(float fovy, float aspectRatio, float near, float far);
    Vec3 getPosition();
    virtual ~Camera()= default;
};

#endif //CGJM_CAMERA_H
