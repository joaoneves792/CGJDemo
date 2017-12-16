//
// Created by joao on 11/28/17.
//

#ifndef CGJDEMO_PARTICLEPOOL_H
#define CGJDEMO_PARTICLEPOOL_H

#include <list>
#include "SceneGraph/SceneGraph.h"
#include "Particles/Particle.h"
#include "SceneGraph/ParticleEmitterNode.h"


#define PARTICLE_COUNT 1024;

class ParticlePool {
private:
    int _particleCount;
    std::vector<Particle*> _alive;
    std::vector<Particle*> _dead;
    SceneGraph* _scene;

    GLuint _particlesVAO;
    GLuint _vertVBO;
    GLuint _stateVBO;

    GLfloat* _stateBuffer;

private:
    void __draw(ParticleEmitterNode* emitter, int drawCount, int level);
    void init(int maxParticles);
public:
    ParticlePool(int particleCount, SceneGraph* scene);
    ~ParticlePool();
    unsigned int getParticles(unsigned int count, std::list<Particle*>& list);


    void update(int dt);
    void draw(int level);
};


#endif //CGJDEMO_PARTICLEPOOL_H
