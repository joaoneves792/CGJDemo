//
// Created by joao on 11/23/17.
//

#include "LightNode.h"
#include "SceneNode.h"
#include "vec.h"
#include <cmath>

LightNode::LightNode(std::string name) : SceneNode(name) {
    color = Vec3(0.9f, 0.9f, 0.9f);
    cone = Vec4(0.0f, -1.0f, 0.0f, -1.0f);
    attenuation = Vec4(1.0f, 0.0f, 0.0f, -1.0f);
}

void LightNode::setCone(float x, float y, float z, float angle) {
    cone = Vec4(x, y, z, std::cos(angle));
}

void LightNode::setPoint(float x, float y, float z) {
    cone = Vec4(0.0f, 1.0f, 0.0f, -2.0f);
    position = Vec3(x, y, z);
}

void LightNode::setColor(float r, float g, float b) {
    color = Vec3(r, g, b);
}

void LightNode::setAttenuation(float k, float kd, float kdd, float range) {
    attenuation = Vec4(k, kd, kdd, range);
}

Vec4 LightNode::getCone(){
    Mat4 rotation = this->getOrientation().GLMatrix().transpose();
    Vec3 direction = Vec3(cone[0], cone[1], cone[2]);
    direction = rotation * direction;
    return Vec4(direction[0], direction[1], direction[2], cone[3]);
}

Vec3 LightNode::getColor() {
    return color;
}

Vec4 LightNode::getAttenuation(){
    return attenuation;
}

Vec3 LightNode::getPosition(){
    return this->getTranslation()*position;
}