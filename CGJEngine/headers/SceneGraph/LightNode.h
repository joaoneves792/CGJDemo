//
// Created by joao on 11/23/17.
//

#ifndef CGJDEMO_LIGHTNODE_H
#define CGJDEMO_LIGHTNODE_H

#include <functional>
#include <list>
#include <string>

#include "glm_wrapper.h"
#include "Shader.h"
#include "SceneGraph/SceneNode.h"

class LightNode : public SceneNode{
private:
    Vec4 _cone;
    Vec3 _color;
    Vec4 _attenuation;


public:
    LightNode(std::string name);

    void setCone(float x, float y, float z, float angle);
    void setPoint(float x, float y, float z);
    void setColor(float r, float g, float b);
    void setAttenuation(float k, float kd, float kdd, float range);

    Vec3 getPositionWorldspace() override;

    Vec4 getCone();
    Vec3 getColor();
    Vec4 getAttenuation();
};
#endif //CGJDEMO_LIGHTNODE_H
