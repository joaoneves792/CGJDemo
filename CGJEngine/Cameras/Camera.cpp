//
// Created by joao on 11/12/17.
//
#include "Camera.h"
#include "glm_wrapper.h"

void Camera::perspective(float fovy, float aspectRatio, float near, float far) {
    this->_fovy = fovy;
    _near = near;
    _far = far;
    _projection = glm::perspective(fovy, aspectRatio, near, far);
}

Mat4 Camera::getProjectionMatrix() {
    return _projection;
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
    return _position;
}