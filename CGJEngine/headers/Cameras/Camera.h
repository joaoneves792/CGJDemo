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
    Mat4 _inverseProjection;
    float _fovy;
    float _near;
    float _far;

    bool _reflected = false;
    Vec3 _reflectionPlaneN;
    Vec3 _reflectionPlaneP;

protected:
    void calculateBillboard();
public:
    virtual Mat4 getMatrix()=0;
    Mat4 getProjectionMatrix();
    Mat4 getInverseProjection();
    Quat getBillboardOrientation();
    Mat4 getViewMatrix();
    Mat4 getReflectedViewMatrix();
    void setPosition(float x, float y, float z);
    void setOrientation(float w, float x, float y, float z);
    Quat getOrientation();
    virtual Mat4 getInverseViewMatrix()=0;
    virtual void move(float x, float y, float z)=0;
    virtual void changeOrientation(float yaw, float pitch, float roll)=0;
    virtual void resize(int x, int y);
    void perspective(float fovy, float aspectRatio, float near, float far);
    Vec3 getPosition();
    void setReflection(bool reflected, float nx, float ny, float nz, Vec3 p);
    virtual ~Camera()= default;

protected:
    virtual Mat4 produceViewMatrix()=0;
    Mat4 getReflectionMatrix(const Vec3 &N, float D) const;
};

#endif //CGJM_CAMERA_H
