//
// Created by joao on 11/12/17.
//
#include "Camera.h"

void Camera::perspective(float fovy, float aspectRatio, float near, float far) {
    projection = CGJM::perspective(fovy, aspectRatio, near, far);
}

void Camera::ortho(float left, float right, float top, float bottom, float near, float far) {
    projection = CGJM::ortho(left, right, top, bottom, near, far);
}

Mat4 Camera::getProjectionMatrix() {
    return projection;
}