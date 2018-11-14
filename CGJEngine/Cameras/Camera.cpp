//
// Created by joao on 11/12/17.
//
#include <iostream>
#include "Cameras/Camera.h"
#include "glm_wrapper.h"

void Camera::perspective(float fovy, float aspectRatio, float near, float far) {
    this->_fovy = fovy;
    _near = near;
    _far = far;
    _projection = glm::perspective(fovy, aspectRatio, near, far);
    _inverseProjection = glm::inverse(_projection);
}

Mat4 Camera::getProjectionMatrix() {
    return _projection;
}

Mat4 Camera::getInverseProjection() {
    return _inverseProjection;
}

void Camera::resize(int x, int y) {
    perspective(_fovy, (float)x/(float)y, _near, _far);
}

void Camera::calculateBillboard() {
    Mat4 View = glm::toMat4(_orientation);
    Mat4 ViewT = Mat4(View[0][0], View[1][0], View[2][0], 0,
                      View[0][1], View[1][1], View[2][1], 0,
                      View[0][2], View[1][2], View[2][2], 0,
                      0, 0, 0, 1);
    //Convert it into a quat
    _pseudoBillboard = glm::toQuat(ViewT);
}

Quat Camera::getBillboardOrientation() {
    return _pseudoBillboard;
}

Vec3 Camera::getPosition() {
    Mat4 View = produceViewMatrix();

    Mat3 rotMat = Mat3(View);
    Vec3 d = glm::column(View, 3);

    return -d * rotMat;
}

Mat4 Camera::getReflectedViewMatrix() {
    Vec3 N = _reflectionPlaneN;
    Vec3 P = _reflectionPlaneP;
    float D = glm::dot(-P, N);
    Mat4 reflectionM = getReflectionMatrix(N, D);
    return produceViewMatrix()*reflectionM;
}

Mat4 Camera::getViewMatrix() {
    if(!_reflected)
        return produceViewMatrix();
    else
        return getReflectedViewMatrix();
}

Mat4 Camera::getReflectionMatrix(const Vec3 &N, float D) const {
    Mat4 r = Mat4(1.0f - 2.0f * N[0] * N[0],
                -2.0f*N[0]*N[1],
                -2.0f*N[0]*N[2],
                -2.0f*N[0]*D,
                //  1.0f,

                -2.0f*N[0]*N[1],
                1.0f-2.0f*N[1]*N[1],
                -2.0f*N[1]*N[2],
                -2.0f*N[1]*D,
                //  1.0f,

                -2.0f*N[0]*N[2],
                -2.0f*N[1]*N[2],
                1.0f-2.0f*N[2]*N[2],
                -2.0f*N[2]*D,
                //  1.0f,

                0.0f,
                0.0f,
                0.0f,
                1.0f);
    return glm::transpose(r);
}

void Camera::setReflection(bool reflected, float nx, float ny, float nz, Vec3 p) {
    _reflected = reflected;
    _reflectionPlaneN = Vec3(nx, ny, nz);
    _reflectionPlaneP = p;
}

void Camera::setPosition(float x, float y, float z) {
    _position[0] = x;
    _position[1] = y;
    _position[2] = z;
}

void Camera::setOrientation(float w, float x, float y, float z) {
    _orientation[0] = w;
    _orientation[1] = x;
    _orientation[2] = y;
    _orientation[3] = z;
}

Quat Camera::getOrientation() {
    return _orientation;
}
