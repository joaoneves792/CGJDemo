//
// Created by joao on 11/12/17.
//

#include <iostream>
#include "Cameras/FreeCamera.h"
#include "glm_wrapper.h"

FreeCamera::FreeCamera(Vec3 originalPosition, Quat originalOrientation) {
    _position = originalPosition;
    _orientation = originalOrientation;
    calculateBillboard();

}
Mat4 FreeCamera::getMatrix() {
    return _projection * produceViewMatrix();
}

Mat4 FreeCamera::produceViewMatrix() {
    return glm::toMat4(_orientation) * glm::translate(Mat4(1.0f), Vec3(-_position[0], -_position[1], -_position[2]));
}

void FreeCamera::move(float x, float y, float z){
    _position += Vec3(x, y, z) * _orientation;
}
void FreeCamera::changeOrientation(float yaw, float pitch, float roll){
    _orientation = glm::angleAxis(yaw, up) * _orientation;
    _orientation = glm::angleAxis(pitch, right) * _orientation;
    _orientation = glm::angleAxis(roll, front) * _orientation;
    glm::normalize(_orientation);

    calculateBillboard();
}

void FreeCamera::resize(int x, int y) {
    Camera::resize(x, y);
}

FreeCamera::~FreeCamera()=default;
