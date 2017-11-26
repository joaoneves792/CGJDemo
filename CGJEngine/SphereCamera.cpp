//
// Created by joao on 11/12/17.
//

#include "SphereCamera.h"
#include "glm_wrapper.h"

SphereCamera::SphereCamera(float distance, Vec3 center, Quat originalOrientation) {
    _position = center;
    this->_distance = distance;
    _orientation = originalOrientation;
}
Mat4 SphereCamera::getMatrix() {
    return _projection * getViewMatrix();
}

Mat4 SphereCamera::getViewMatrix() {
    return glm::translate(Mat4(1.0f), Vec3(0, 0, -_distance))
           * glm::translate(glm::toMat4(_orientation), Vec3(-_position[0], -_position[1], -_position[2]));
}

void SphereCamera::move(float x, float y, float z){
    _distance = _distance + z;
}
void SphereCamera::changeOrientation(float yaw, float pitch, float roll){
    _orientation = glm::angleAxis(yaw, up) * _orientation;
    _orientation = glm::angleAxis(pitch, right) * _orientation;
    _orientation = glm::angleAxis(roll, front) * _orientation;
    glm::normalize(_orientation);
}

void SphereCamera::resize(int x, int y) {
    Camera::resize(x, y);
}
SphereCamera::~SphereCamera()=default;
