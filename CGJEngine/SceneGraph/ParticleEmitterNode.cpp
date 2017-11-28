//
// Created by joao on 11/28/17.
//

#include <iostream>
#include "ParticleEmitterNode.h"
#include "ParticlePool.h"

ParticleEmitterNode::ParticleEmitterNode(std::string name, ParticlePool* pool, Shader *shader, GLuint texture) :SceneNode(name){
    _shader = shader;
    _texture = texture;
    _pool = pool;
    _processingLevel = DEFAULT_PARTICLES_LEVEL;
    _acceleration = Vec3(0.0f, 0.0f, 0.0f);
    _velocity = Vec3(0.0f, 0.0f, 0.0f);
    _randomness = Vec3(0.0f, 0.0f, 0.0f);
    _rate = 1.0f;
    _rateDecay = 0.1f;
    _currentRate = 0;
    _billboard = true;

    _lifeDecayRate = 0.001f;
}

void ParticleEmitterNode::setAcceleration(const Vec3 &a) {
    _acceleration = a;
}

void ParticleEmitterNode::setVelocity(const Vec3 &v) {
    _velocity = v;
}

void ParticleEmitterNode::setRandomness(const Vec3 &r) {
    _randomness = r;
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

void ParticleEmitterNode::update(int dt) {
    if(_updateCallback != nullptr)
        _updateCallback(dt);

    if(_currentRate > 0) {
        Vec3 position = getPosition();

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
        for (int i = 0; i < (int) newParticlesCount; i++) {
            Particle *p = _pool->getParticle();
            if (p != nullptr) {
                p->emitterNode = this;
                p->level = _processingLevel;
                p->life = 1.0f;
                p->position = position;
                p->velocity = _velocity;
                _particles.push_back(p);
            }
        }
    }
    /*Update particles*/
    //TODO take randomness into consideration
    auto it = _particles.begin();
    while(it!=_particles.end()){
        Particle *p = (*it);
        p->velocity += (_acceleration * (float)dt);
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

void ParticleEmitterNode::draw(int level) {
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

        glBindTexture(GL_TEXTURE_2D, _texture);
    }
}

void ParticleEmitterNode::postDraw(int level) {
    glUseProgram(0);

    //Call post draw
    if (_postDraw != nullptr)
        _postDraw();

    //Draw the childs
    for(SceneNode* n : _childs)
        n->draw(level);
}

ParticleEmitterNode::~ParticleEmitterNode() {
    for(auto p : _particles){
        p->life = 0.0f;
    }
    _particles.clear();
}