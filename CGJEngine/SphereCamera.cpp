//
// Created by joao on 11/12/17.
//

#include "SphereCamera.h"
#include "quat.h"
#include "mat.h"
#include "vec.h"

SphereCamera::SphereCamera(float distance, Vec3 center, Quat originalOrientation) {
    position = center;
    this->distance = distance;
    orientation = originalOrientation;
}
Mat4 SphereCamera::getMatrix() {
    return projection * getViewMatrix();
}

Mat4 SphereCamera::getViewMatrix() {
    return CGJM::translate(0, 0, -distance) * orientation.GLMatrix().transpose()
           * CGJM::translate(-position[0], -position[1], -position[2]);
}

void SphereCamera::move(float x, float y, float z){
    distance = distance + z;
}
void SphereCamera::changeOrientation(float yaw, float pitch, float roll){
    orientation = Quat(yaw, up) * orientation;
    orientation = Quat(pitch, right) * orientation;
    orientation = Quat(roll, front) * orientation;
}

SphereCamera::~SphereCamera()=default;
