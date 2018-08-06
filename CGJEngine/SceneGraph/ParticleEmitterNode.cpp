//
// Created by joao on 11/28/17.
//

#include <climits>
#include <iostream>
#include <random>
#include "SceneGraph/ParticleEmitterNode.h"
#include "Particles/ParticlePool.h"

ParticleEmitterNode::ParticleEmitterNode(std::string name, ParticlePool* pool, Shader *shader, Texture* texture) :SceneNode(name){
    _shader = shader;
    _texture = texture;
    _pool = pool;
    _processingLevel = DEFAULT_PARTICLES_LEVEL;
    _acceleration = Vec3(0.0f, 0.0f, 0.0f);
    _velocity = Vec3(0.0f, 0.0f, 0.0f);
    _randomAcceleration = Vec3(0.0f, 0.0f, 0.0f);
    _rate = 1.0f;
    _rateDecay = 0.1f;
    _currentRate = 0;
    _billboard = false;

    _lifeDecayRate = 0.001f;
}

void ParticleEmitterNode::setAcceleration(const Vec3 &a) {
    _acceleration = a;
}

void ParticleEmitterNode::setVelocity(const Vec3 &v) {
    _velocity = v;
}

void ParticleEmitterNode::setRandomAcceleration(const Vec3 &r) {
    _randomAcceleration = r;
}

void ParticleEmitterNode::setParticleLifeDecayRate(float lifeDecay) {
    _lifeDecayRate = lifeDecay;
}

void ParticleEmitterNode::setRate(float ppms) {
    _rate = ppms;
}

void ParticleEmitterNode::setRateDecay(float rateDecay) {
    _rateDecay = rateDecay;
}

void ParticleEmitterNode::emmit() {
    _currentRate = _rate;
}


float randomFloat(){ //xorshift96
    static unsigned long x=123456789, y=362436069, z=521288629;
    unsigned long t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;

    return (long)z/(float)LONG_MAX;
}

/*float randomFloat(){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, RAND_MAX);

    return ((float)dis(gen))/((float)RAND_MAX);
}*/

void ParticleEmitterNode::update(int dt) {

    if(_updateCallback != nullptr)
        _updateCallback(dt);

    if(_currentRate > 0) {
        Vec3 position = getPositionWorldspace();

        /*Calculate how many new particles we need*/
        static float leftOverParticles = 0.0f;
        float newParticlesCount = _currentRate * dt;
        leftOverParticles += newParticlesCount - (int) newParticlesCount;
        if (leftOverParticles >= 1.0f) {
            newParticlesCount += (int) leftOverParticles;
            leftOverParticles = leftOverParticles - (int) leftOverParticles;
        }

        _currentRate -= _rateDecay * dt;

        /*Add new particles*/
        unsigned int returnedParticlesCount = _pool->getParticles((unsigned int)newParticlesCount, _particles);
        auto rit = _particles.rbegin();
        for (unsigned int i = 0; i < returnedParticlesCount; i++) {
            Particle *p = (*rit);
            p->emitterNode = this;
            p->level = _processingLevel;
            p->life = 1.0f;
            p->position = position;
            p->velocity = _velocity;
            p->acceleration = _acceleration + Vec3(_randomAcceleration[0]*randomFloat(), _randomAcceleration[1]*randomFloat(), _randomAcceleration[2]*randomFloat());
            rit++;
        }
    }
    /*Update particles*/
    auto it = _particles.begin();
    while(it!=_particles.end()){
        Particle *p = (*it);
        p->velocity += (p->acceleration * (float)dt);
        p->position += (p->velocity * (float) dt);
        p->life -= _lifeDecayRate * dt;
        if(p->life <= 0){
            _particles.erase(it++);
        }else{
            ++it;
        }
    }

    for(SceneNode* n : _childs)
        n->update(dt);
}

void ParticleEmitterNode::particlePreDraw(int level) {
    if(!_visible)
        return;

    if(level == _processingLevel) {
        _shader->use();

        //Call pre draw
        if (_preDraw != nullptr)
            _preDraw();

        Mat4 P = _scene->getProjectionMatrix();
        Mat4 V = _scene->getViewMatrix();
        Mat4 M = getModelMatrix();
        _shader->uploadMVP(M, V, P);

        if(_texture){
            glActiveTexture(GL_TEXTURE0);
            _texture->bind();
        }
    }
}

void ParticleEmitterNode::particlePostDraw(int level) {
    glUseProgram(0);

    //Call post draw
    if (_postDraw != nullptr)
        _postDraw();

}

ParticleEmitterNode::~ParticleEmitterNode() {
    for(auto p : _particles){
        p->life = 0.0f;
    }
    _particles.clear();
}