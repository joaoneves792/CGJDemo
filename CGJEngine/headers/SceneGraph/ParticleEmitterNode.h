//
// Created by joao on 11/28/17.
//

#ifndef CGJDEMO_PARTICLEEMITTERNODE_H
#define CGJDEMO_PARTICLEEMITTERNODE_H

#include <list>

#include "Shader.h"
#include "SceneGraph/SceneNode.h"
#include "Particles/Particle.h"
#include "Textures/Texture.h"

class ParticlePool;

class ParticleEmitterNode : public SceneNode {
private:
    Vec3 _acceleration;
    Vec3 _velocity;
    Vec3 _randomAcceleration;
    Texture* _texture;

    float _rate;
    float _rateDecay;
    float _currentRate;

    float _lifeDecayRate;

    ParticlePool* _pool;
    std::list<Particle*> _particles;

public:
    ParticleEmitterNode(std::string name, ParticlePool* pool, Shader* shader, Texture* texture);
    virtual ~ParticleEmitterNode();
    void setAcceleration(const Vec3& a);
    void setVelocity(const Vec3& v);
    void setRandomAcceleration(const Vec3 &r);
    void setRate(float ppms);
    void setRateDecay(float rateDecay);
    void setParticleLifeDecayRate(float lifeDecay);
    void emmit();

    void particlePreDraw(int level);
    void particlePostDraw(int level);

    virtual void update(int dt);
};


#endif //CGJDEMO_PARTICLEEMITTERNODE_H
