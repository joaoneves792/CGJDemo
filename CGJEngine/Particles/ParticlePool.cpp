//
// Created by joao on 11/28/17.
//

#include <iostream>
#include <algorithm>
#include "ParticlePool.h"
#include "SceneGraph.h"
#include "ParticleEmitterNode.h"
#include "Particle.h"

void ParticlePool::init(int maxParticles) {
    static const GLfloat vertexBufferData[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
    };
    glGenVertexArrays(1, &_particlesVAO);
    glBindVertexArray(_particlesVAO);

    glGenBuffers(1, &_vertVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _vertVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

    glGenBuffers(1, &_posVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _posVBO);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 3 * sizeof(GLfloat), 0, GL_STREAM_DRAW);

    glGenBuffers(1, &_lifeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _lifeVBO);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(GLfloat), 0, GL_STREAM_DRAW);


    glEnableVertexAttribArray(PARTICLE_VERT_ATTR);
    glBindBuffer(GL_ARRAY_BUFFER, _vertVBO);
    glVertexAttribPointer(PARTICLE_VERT_ATTR, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(PARTICLE_POS_ATTR);
    glBindBuffer(GL_ARRAY_BUFFER, _posVBO);
    glVertexAttribPointer(PARTICLE_POS_ATTR, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(PARTICLE_LIFE_ATTR);
    glBindBuffer(GL_ARRAY_BUFFER, _lifeVBO);
    glVertexAttribPointer(PARTICLE_LIFE_ATTR, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

ParticlePool::ParticlePool(int particleCount, SceneGraph* scene) {
    _particleCount = particleCount;
    _alive.reserve((unsigned long)particleCount);
    _dead.reserve((unsigned long)particleCount);
    for(int i=0; i<particleCount; i++)
        _dead.push_back(new Particle);
    _positionBuffer = new GLfloat[particleCount*3];
    _lifeBuffer = new GLfloat[particleCount*1];
    init(particleCount);
    _scene = scene;
}

ParticlePool::~ParticlePool() {
    for(auto p : _alive){
        delete p;
    }
    for(auto p : _dead){
        delete p;
    }
    _alive.clear();
    _dead.clear();
    delete[] _positionBuffer;
    delete[] _lifeBuffer;
}

Particle* ParticlePool::getParticle() {
    Particle *p = nullptr;
    if(!_dead.empty()) {
        p = _dead.back();
        _dead.pop_back();
        _alive.push_back(p);
    }else{
        std::cerr << "Active particle limit exceeded!" << std::endl;
    }
    return p;
}


void ParticlePool::update(int dt) {
    //Sort all alive particles
    Vec3 cameraPosition = _scene->getCamera()->getPosition();
    std::sort(_alive.begin(), _alive.end(), [=](Particle* a, Particle* b){
        if(b->life <= 0)
            return true;
        Vec3 va = cameraPosition - a->position;
        Vec3 vb = cameraPosition - b->position;
        return (glm::dot(va, va) < glm::dot(vb, vb)); //Ignore the error in CLion
    });

    //Clear out dead particles //If everything works correctly they will be the last elements in the vector
    if(!_alive.empty()) {
        auto it = _alive.rbegin();
        while ((*it)->life <= 0.0f) {
            _dead.push_back(*it);
            _alive.pop_back();
            it++;
        }
    }
}

void ParticlePool::__draw(ParticleEmitterNode* emitterNode, int drawCount, int level) {
    emitterNode->draw(level);

    glBindVertexArray(_particlesVAO);

    glBindBuffer(GL_ARRAY_BUFFER, _posVBO);
    glBufferData(GL_ARRAY_BUFFER, _particleCount * 3 * sizeof(GLfloat), 0, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, drawCount * sizeof(GLfloat) * 3, _positionBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, _lifeVBO);
    glBufferData(GL_ARRAY_BUFFER, _particleCount * sizeof(GLfloat), 0, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, drawCount * sizeof(GLfloat), _lifeBuffer);

    glVertexAttribDivisor(PARTICLE_VERT_ATTR, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(PARTICLE_POS_ATTR, 1); // positions : one per quad (its center) -> 1
    glVertexAttribDivisor(PARTICLE_LIFE_ATTR, 1); // color : one per quad -> 1

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, drawCount);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    emitterNode->postDraw(level);
}

void ParticlePool::draw(int level) {
    if(_alive.empty())
        return;

    ParticleEmitterNode* lastEmitter = _alive.front()->emitterNode;
    int instanceCount = 0;
    for(auto p : _alive){
        if(p->level != level)
            continue;
        //Check for boundaries
        if(p->emitterNode == lastEmitter)
            ++instanceCount;
        else{
            __draw(lastEmitter, instanceCount, level);
            instanceCount = 1;
        }
        //Update buffer info
        int posi = (instanceCount-1)*3;
        _positionBuffer[posi++] = p->position[0];
        _positionBuffer[posi++] = p->position[1];
        _positionBuffer[posi] = p->position[2];
        _lifeBuffer[instanceCount] = p->life;
    }
    if(instanceCount > 0)
        __draw(lastEmitter, instanceCount, level);
}

