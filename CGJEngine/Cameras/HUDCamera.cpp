//
// Created by joao on 11/12/17.
//

#include <iostream>
#include "Cameras/HUDCamera.h"
#include "glm_wrapper.h"

HUDCamera::HUDCamera(float left, float right, float top, float bottom, float near, float far, bool scale) {
    _near = near;
    _far = far;
    _left = left;
    _right = right;
    _top = top;
    _bottom = bottom;
    _scale = scale;
    _projection = glm::ortho(left, right, bottom, top, near, far);
    _inverseProjection = glm::inverse(_projection);
    _pseudoBillboard = Quat(1.0f, 0.0f, 0.0f, 0.0f);//Identity quaternion
    _position = Vec3(0.0f, 0.0f, 0.0f);
    _orientation = Quat(1.0f, 0.0f, 0.0f, 0.0f);
}

Mat4 HUDCamera::getMatrix() {
    return _projection;
}

Mat4 HUDCamera::produceViewMatrix() {
    //std::cout << _orientation[0] << " " <<  _orientation[1] << " " << _orientation[2] << " " << _orientation[3] << std::endl;
    //std::cout << _position[0] << " " << _position[1] << " " << _position[2] << std::endl;
    return glm::toMat4(_orientation) * glm::translate(Mat4(1.0f), Vec3(-_position[0], -_position[1], -_position[2]));
}

Mat4 HUDCamera::getInverseViewMatrix() {
    return glm::toMat4(glm::inverse(_orientation)) *
           glm::translate(Mat4(1.0f), Vec3(_position[0], _position[1], _position[2]));
}

void HUDCamera::resize(int x, int y) {
    if(_scale)
        return;
    float topRatio = std::abs(_top/ ( std::abs(_top)+std::abs(_bottom) ) );
    float bottomRatio = std::abs(_bottom/ ( std::abs(_bottom)+std::abs(_top) ) );
    float leftRatio = std::abs(_left/ ( std::abs(_left)+std::abs(_right) ) );
    float rightRatio = std::abs(_right/ ( std::abs(_right)+std::abs(_left) ) );

    _left = (_left)? _left*std::abs(x/_left)*leftRatio : 0;
    _right = (_right)? _right*std::abs(x/_right)*rightRatio : 0;
    _top = (_top)? _top*std::abs(y/_top)*topRatio : 0;
    _bottom = (_bottom)? _bottom*std::abs(y/_bottom)*bottomRatio : 0;

    _projection = glm::ortho(_left, _right, _bottom, _top, _near, _far);
    _inverseProjection = glm::inverse(_projection);
}

void HUDCamera::move(float x, float y, float z) {
    _position += Vec3(x, y, z);
}

void HUDCamera::changeOrientation(float yaw, float pitch, float roll) {
    _orientation = glm::angleAxis(yaw, up) * _orientation;
    _orientation = glm::angleAxis(pitch, right) * _orientation;
    _orientation = glm::angleAxis(roll, front) * _orientation;
    glm::normalize(_orientation);
}

HUDCamera::~HUDCamera()=default;
