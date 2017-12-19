//
// Created by joao on 11/23/17.
//

#include "SceneGraph/LightNode.h"
#include "SceneGraph/SceneNode.h"
#include "glm_wrapper.h"
#include <cmath>

LightNode::LightNode(std::string name) : SceneNode(name) {
    _color = Vec3(0.9f, 0.9f, 0.9f);
    _cone = Vec4(0.0f, -1.0f, 0.0f, -1.0f);
    _attenuation = Vec4(1.0f, 0.0f, 0.0f, -1.0f);
}

void LightNode::setCone(float x, float y, float z, float angle) {
    _cone = Vec4(x, y, z, std::cos(angle));
}

void LightNode::setPoint(float x, float y, float z) {
    _cone = Vec4(0.0f, 1.0f, 0.0f, -2.0f);
    _position = Vec3(x, y, z);
}

void LightNode::setColor(float r, float g, float b) {
    _color = Vec3(r, g, b);
}

void LightNode::setAttenuation(float k, float kd, float kdd, float range) {
    _attenuation = Vec4(k, kd, kdd, range);
}

Vec4 LightNode::getCone(){
    Mat4 rotation = glm::toMat4(this->getOrientation());
    Vec4 direction = Vec4(_cone[0], _cone[1], _cone[2], 0.0f);
    direction = rotation * direction;
    return Vec4(direction[0], direction[1], direction[2], _cone[3]);
}

Vec3 LightNode::getColor() {
    return _color;
}

Vec4 LightNode::getAttenuation(){
    return _attenuation;
}

Vec3 LightNode::getPositionWorldspace(){
    return Vec3(getTranslation()*Vec4(_position, 1.0f));
}

