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