//
// Created by joao on 11/12/17.
//

#include "FreeCamera.h"
#include "quat.h"
#include "mat.h"
#include "vec.h"

FreeCamera::FreeCamera(Vec3 originalPosition, Quat originalOrientation) {
    position = originalPosition;
    orientation = originalOrientation;
    rotated_front = front;
    rotated_right = right;
    rotated_up = up;
}
Mat4 FreeCamera::getMatrix() {
    return projection * getViewMatrix();
}

Mat4 FreeCamera::getViewMatrix() {
    return orientation.GLMatrix().transpose() * CGJM::translate(-position[0], -position[1], -position[2]);
}

void FreeCamera::move(float x, float y, float z){
    position = position - rotated_front*z + rotated_right*x;
}
void FreeCamera::changeOrientation(float yaw, float pitch, float roll){
    orientation = Quat(yaw, up) * orientation;
    orientation = Quat(pitch, right) * orientation;
    orientation = Quat(roll, front) * orientation;

    rotated_right = orientation.GLMatrix() * right;
    rotated_front = orientation.GLMatrix() * front;
}

FreeCamera::~FreeCamera()=default;
