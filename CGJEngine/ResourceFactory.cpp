//
// Created by joao on 11/21/17.
//
#include <iostream>
#include <Textures/Texture.h>
#include <FBOs/MSFrameBuffer.h>
#include <Cameras/VRCamera.h>
#include "ResourceManager.h"
#include "Meshes/Mesh.h"
#include "Meshes/OBJMesh.h"
#include "Meshes/H3DMesh.h"
#include "Shader.h"
#include "SceneGraph/SceneGraph.h"
#include "SceneGraph/LightNode.h"
#include "Lights/LightsManager.h"
#include "Cameras/Camera.h"
#include "Cameras/FreeCamera.h"
#include "Cameras/SphereCamera.h"

Mesh* ResourceManager::Factory::createMesh(const std::string &name, const std::string &filename) {
    if(filename.substr(filename.find_last_of('.')+1) == ("h3d")){
        auto mesh = new H3DMesh(filename);
        ResourceManager::getInstance()->addMesh(name, mesh);
        return mesh;
    }else if(filename.substr(filename.find_last_of('.')+1) == ("obj")){
        auto mesh = new OBJMesh(filename);
        ResourceManager::getInstance()->addMesh(name, mesh);
        return mesh;
    }

    std::cerr << "Unrecognized mesh format " << filename << std::endl;
    exit(EXIT_FAILURE);
}

Shader* ResourceManager::Factory::createShader(const std::string &name, const std::string &vertexShader,
                                      const std::string& fragmentShader) {
    auto shader = new Shader(vertexShader.c_str(), fragmentShader.c_str());
    ResourceManager::getInstance()->addShader(name, shader);
    return shader;
}

SceneNode* ResourceManager::Factory::createScene(const std::string &name, Camera *camera) {
    auto scene = new SceneGraph(camera);
    ResourceManager::getInstance()->addScene(name, scene);
    return scene->getRoot();
}

LightNode* ResourceManager::Factory::createLight(const std::string &name) {
    auto light = new LightNode(name);
    LightsManager::getInstance()->addLight(light);
    return light;
}

FreeCamera* ResourceManager::Factory::createFreeCamera(const std::string &name, Vec3 position, Quat orientation) {
    auto camera = new FreeCamera(position, orientation);
    ResourceManager::getInstance()->addCamera(name, camera);
    return camera;
}

SphereCamera* ResourceManager::Factory::createSphereCamera(const std::string &name,
                                                           float distance, Vec3 center, Quat orientation) {
    auto camera = new SphereCamera(distance, center, orientation);
    ResourceManager::getInstance()->addCamera(name, camera);
    return camera;
}

HUDCamera* ResourceManager::Factory::createHUDCamera(const std::string &name, float left, float right, float top,
                                                     float bottom, float near, float far, bool scale) {
    auto camera = new HUDCamera(left, right, top, bottom, near, far, scale);
    ResourceManager::getInstance()->addCamera(name, camera);
    return camera;
}

VRCamera* ResourceManager::Factory::createVRCamera(const std::string &name, Vec3 position, Quat orientation) {
    auto camera = new VRCamera(position, orientation);
    ResourceManager::getInstance()->addCamera(name, camera);
    return camera;
}

FrameBuffer* ResourceManager::Factory::createFrameBuffer(const std::string &name, int x, int y) {
    auto fbo = new FrameBuffer(x, y);
    ResourceManager::getInstance()->addFrameBuffer(name, fbo);
    return fbo;
}

GFrameBuffer* ResourceManager::Factory::createGFrameBuffer(const std::string &name, int x, int y) {
    auto fbo = new GFrameBuffer(x, y);
    ResourceManager::getInstance()->addFrameBuffer(name, fbo);
    return fbo;
}

MSFrameBuffer* ResourceManager::Factory::createMSAAFrameBuffer(const std::string &name, int x, int y, int msaaLevel) {
    auto fbo = new MSFrameBuffer(x, y, msaaLevel);
    ResourceManager::getInstance()->addFrameBuffer(name, fbo);
    return fbo;
}

DoubleColorMSFrameBuffer* ResourceManager::Factory::createDoubleColorMSAAFrameBuffer(const std::string &name, int x,
                                                                                     int y, int msaaLevel) {
    auto fbo = new DoubleColorMSFrameBuffer(x, y, msaaLevel);
    ResourceManager::getInstance()->addFrameBuffer(name, fbo);
    return fbo;
}

ColorTextureFrameBuffer* ResourceManager::Factory::createColorTextureFrameBuffer(const std::string &name, int x, int y) {
    auto fbo = new ColorTextureFrameBuffer(x, y);
    ResourceManager::getInstance()->addFrameBuffer(name, fbo);
    return fbo;
}

DepthTextureFrameBuffer* ResourceManager::Factory::createDepthTextureFrameBuffer(const std::string &name, int x,
                                                                                 int y) {
    auto fbo = new DepthTextureFrameBuffer(x, y);
    ResourceManager::getInstance()->addFrameBuffer(name, fbo);
    return fbo;
}

ParticlePool* ResourceManager::Factory::createParticlePool(const std::string &name, int particleCount,
                                                           SceneGraph *scene) {
    auto pool = new ParticlePool(particleCount, scene);
    ResourceManager::getInstance()->addParticlePool(name, pool);
    return pool;
}

ParticleEmitterNode* ResourceManager::Factory::createParticleEmmiter(const std::string &name, ParticlePool *pool,
                                                                     Shader *shader, Texture* texture, Vec3 acceleration,
                                                                     Vec3 velocity, Vec3 position, float rate,
                                                                     float rateDecay) {
    auto emitter = new ParticleEmitterNode(name, pool, shader, texture);
    emitter->setAcceleration(acceleration);
    emitter->setVelocity(velocity);
    emitter->setPosition(position[0], position[1], position[2]);
    emitter->setRate(rate);
    emitter->setRateDecay(rateDecay);
    return emitter;
}

Texture* ResourceManager::Factory::createTexture(const std::string &fileName) {
    Texture* texture = ResourceManager::getInstance()->getTexture(fileName);
    if(texture){
        return texture;
    }

    texture = new Texture(fileName);
    ResourceManager::getInstance()->addTexture(fileName, texture);
    return texture;
}

Texture* ResourceManager::Factory::createCubeMap(const std::string &name, const std::string &right,
                                                 const std::string &left, const std::string &top,
                                                 const std::string &bottom, const std::string &back,
                                                 const std::string &front) {
    Texture* texture = new Texture(right, left, top, bottom, back, front);
    ResourceManager::getInstance()->addTexture(name, texture);
    return texture;
}