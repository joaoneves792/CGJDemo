//
// Created by joao on 11/28/17.
//

#ifndef CGJDEMO_PARTICLE_H
#define CGJDEMO_PARTICLE_H


#include <glm_wrapper.h>
#include <GL/glew.h>


class ParticleEmitterNode;

struct Particle {
    Vec3 acceleration;
    Vec3 position;
    Vec3 velocity;
    float life;
    ParticleEmitterNode* emitterNode;
    int level;
};


#endif //CGJDEMO_PARTICLE_H
