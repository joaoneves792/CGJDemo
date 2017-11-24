//
// Created by joao on 11/12/17.
//

#include "HUDCamera.h"
#include "mat.h"

HUDCamera::HUDCamera(float left, float right, float top, float bottom, float near, float far) {
    _near = near;
    _far = far;
    projection = CGJM::ortho(left, right, top, bottom, near, far);
}

Mat4 HUDCamera::getMatrix() {
    return projection;
}

Mat4 HUDCamera::getViewMatrix() {
    return Mat4(1.0f);
}

void HUDCamera::resize(int x, int y) {
    //empty
}

void HUDCamera::move(float x, float y, float z) {
    //empty
}

void HUDCamera::changeOrientation(float yaw, float pitch, float roll) {
    //empty
}

HUDCamera::~HUDCamera()=default;
