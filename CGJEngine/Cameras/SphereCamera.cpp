//
// Created by joao on 11/12/17.
//

#include "Cameras/SphereCamera.h"
#include "glm_wrapper.h"

SphereCamera::SphereCamera(float distance, Vec3 center, Quat originalOrientation) {
    _center = center;
    _distance = distance;
    _orientation = originalOrientation;
    calculateBillboard();
}
Mat4 SphereCamera::getMatrix() {
    return _projection * produceViewMatrix();
}

Mat4 SphereCamera::produceViewMatrix() {
    return glm::translate(Mat4(1.0f), Vec3(0, 0, -_distance))
           * glm::translate(glm::toMat4(_orientation), Vec3(-_center[0], -_center[1], -_center[2]));
}

void SphereCamera::move(float x, float y, float z){
    _distance = _distance + z;
}
void SphereCamera::changeOrientation(float yaw, float pitch, float roll){
    _orientation = glm::angleAxis(yaw, up) * _orientation;
    _orientation = glm::angleAxis(pitch, right) * _orientation;
    _orientation = glm::angleAxis(roll, front) * _orientation;
    glm::normalize(_orientation);

    calculateBillboard();
}

void SphereCamera::resize(int x, int y) {
    Camera::resize(x, y);
}
SphereCamera::~SphereCamera()=default;
