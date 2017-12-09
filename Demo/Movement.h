//
// Created by joao on 11/21/17.
//

#ifndef CGJDEMO_ROAD_H
#define CGJDEMO_ROAD_H


#include "CGJengine.h"

#define ACCELERATION 15.0f/(1000.0f*1000.0f) //15 units per second^2
#define DECELERATION -60.0f/(1000.0f*1000.0f) //15 units per second^2
#define MAX_SPEED 120.0f/1000.0f //120 units per second
#define WHEEL_RADIUS 1.0f //Units
#define WHEEL_CIRCUMFERENCE WHEEL_RADIUS*2.0f*PI


class Movement {
private:
    static float speed;
    static float z;
public:

    static void accelerate(int dt);
    static void deccelerate(int dt);
};


#endif //CGJDEMO_ROAD_H
