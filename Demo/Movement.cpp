//
// Created by joao on 11/21/17.
//

#include "Movement.h"
#include "Constants.h"
#include "SceneGraph/SceneNode.h"

float Movement::speed = 0;
float Movement::z = 0;

void Movement::accelerate(int dt){
    static SceneNode* road = ResourceManager::getInstance()->getScene(SCENE)->findNode(ROAD);
    static SceneNode* frontAxis = ResourceManager::getInstance()->getScene(SCENE)->findNode(FRONT_AXIS);
    static SceneNode* rearAxis = ResourceManager::getInstance()->getScene(SCENE)->findNode(REAR_AXIS);
    static Shader* shader = ResourceManager::getInstance()->getShader(H3D_SHADER);
    static GLint loc = shader->getUniformLocation("movement");

    if(speed < MAX_SPEED)
        speed += ACCELERATION * dt;
    else
        speed = MAX_SPEED;

    float dz = speed * dt;

    z += dz;
    if(z >= ROAD_LENGTH){
        z -= ROAD_LENGTH;
    }
    road->setPosition(0.0f, 0.0f, z);

    float wheelAngle = (2*PI)*(dz/WHEEL_CIRCUMFERENCE);
    frontAxis->rotate(-1.0f, 0.0f, 0.0f, wheelAngle);
    rearAxis->rotate(-1.0f, 0.0f, 0.0f, wheelAngle);

    shader->use();
    glUniform1f(loc, -z/ROAD_LENGTH);
}

void Movement::deccelerate(int dt) {
    static SceneNode* road = ResourceManager::getInstance()->getScene(SCENE)->findNode(ROAD);
    static SceneNode* frontAxis = ResourceManager::getInstance()->getScene(SCENE)->findNode(FRONT_AXIS);
    static SceneNode* rearAxis = ResourceManager::getInstance()->getScene(SCENE)->findNode(REAR_AXIS);
    static Shader* shader = ResourceManager::getInstance()->getShader(H3D_SHADER);
    static GLint loc = shader->getUniformLocation("movement");

    if(speed > 0.0f)
        speed += DECELERATION * dt;
    else
        speed = 0.0f;

    float dz = speed * dt;

    z += dz;
    if(z >= ROAD_LENGTH){
        z -= ROAD_LENGTH;
    }
    road->setPosition(0.0f, 0.0f, z);

    float wheelAngle = (2*PI)*(dz/WHEEL_CIRCUMFERENCE);
    frontAxis->rotate(-1.0f, 0.0f, 0.0f, wheelAngle);
    rearAxis->rotate(-1.0f, 0.0f, 0.0f, wheelAngle);

    shader->use();
    glUniform1f(loc, -z/ROAD_LENGTH);
}

