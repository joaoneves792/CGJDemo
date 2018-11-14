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

Mat4 SphereCamera::getInverseViewMatrix() {
    //TODO: try not to invert the matrix
    return glm::inverse(produceViewMatrix());
}

void SphereCamera::move(float x, float y, float z){
    _distance = _distance + z;
}

void SphereCamera::moveCenter(float x, float y, float z) {
    _center[0] = _center[0] + x;
    _center[1] = _center[1] + y;
    _center[2] = _center[2] + z;
}

void SphereCamera::changeOrientation(float yaw, float pitch, float roll){
#define lerp(a, b, x) ( a + x * (b - a))
    /*glm::quat q = glm::quat(glm::vec3(pitch, yaw, roll));
    _orientation = q * _orientation;
    glm::normalize(_orientation);*/

    /*static Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
    static Vec3 forward = Vec3(0.0f, 0.0f, -1.0f);
    static Vec3 right = Vec3(1.0f, 0.0f, 0.0f);*/

    _orientation = glm::angleAxis(roll, front) * _orientation;
    _orientation = glm::angleAxis(pitch, right) * _orientation;
    _orientation = _orientation*glm::angleAxis(yaw, up);

    /*glm::normalize(_orientation);
    Vec3 rotatedUp = glm::normalize(glm::rotate(_orientation, up));
    float frontProjection = glm::dot(rotatedUp, front);
    _orientation = glm::angleAxis(pitch, front) * _orientation;

    float x = frontProjection*0.5f+0.5f;*/


    glm::normalize(_orientation);

    calculateBillboard();
}

void SphereCamera::resize(int x, int y) {
    Camera::resize(x, y);
}
SphereCamera::~SphereCamera()=default;
