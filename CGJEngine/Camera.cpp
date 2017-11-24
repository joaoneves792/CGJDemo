//
// Created by joao on 11/12/17.
//
#include "Camera.h"

void Camera::perspective(float fovy, float aspectRatio, float near, float far) {
    this->fovy = fovy;
    _near = near;
    _far = far;
    projection = CGJM::perspective(fovy, aspectRatio, near, far);
}

Mat4 Camera::getProjectionMatrix() {
    return projection;
}

void Camera::resize(int x, int y) {
    perspective(fovy, x/y, _near, _far);
}