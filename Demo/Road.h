//
// Created by joao on 11/21/17.
//

#ifndef CGJDEMO_ROAD_H
#define CGJDEMO_ROAD_H


#include "CGJengine.h"

class Road {
private:
    static float ACCELERATION; //15 units per second^2
    static float DECELERATION; //15 units per second^2
    static float MAX_SPEED; //120 units per second

    static float speed;
    static float z;
public:

    static void accelerate(int dt);
    static void deccelerate(int dt);
};


#endif //CGJDEMO_ROAD_H
