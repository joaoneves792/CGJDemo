//
// Created by joao on 11/21/17.
//

#include "Road.h"
#include "ResourceNames.h"

float Road::ACCELERATION = 15.0f/(1000.0f*1000.0f); //15 units per second^2
float Road::DECELERATION = -60.0f/(1000.0f*1000.0f); //15 units per second^2
float Road::MAX_SPEED = 120.0f/1000.0f; //120 units per second

float Road::speed = 0;
float Road::z = 0;

void Road::accelerate(int dt){
    static SceneNode* road = ResourceManager::getInstance()->getScene(SCENE)->findNode(ROAD);

    if(speed < MAX_SPEED)
        speed += ACCELERATION * dt;
    else
        speed = MAX_SPEED;

    float dz = speed * dt;

    z += dz;
    if(z >= 60.0f){
        z -= 60.0f;
    }
    road->setPosition(0.0f, 0.0f, z);
}

void Road::deccelerate(int dt) {
    static SceneNode* road = ResourceManager::getInstance()->getScene(SCENE)->findNode(ROAD);

    if(speed > 0.0f)
        speed += DECELERATION * dt;
    else
        speed = 0.0f;

    float dz = speed * dt;

    z += dz;
    if(z >= 60.0f){
        z -= 60.0f;
    }
    road->setPosition(0.0f, 0.0f, z);
}

