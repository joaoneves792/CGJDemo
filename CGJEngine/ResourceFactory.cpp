//
// Created by joao on 11/21/17.
//
#include <iostream>
#include "ResourceManager.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "H3DMesh.h"
#include "Shader.h"
#include "SceneGraph.h"
#include "LightNode.h"
#include "LightsManager.h"
#include "Camera.h"
#include "FreeCamera.h"
#include "SphereCamera.h"

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
                                      const std::string fragmentShader) {
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
                                                     float bottom, float near, float far) {
    auto camera = new HUDCamera(left, right, top, bottom, near, far);
    ResourceManager::getInstance()->addCamera(name, camera);
    return camera;
}

FrameBuffer* ResourceManager::Factory::createFrameBuffer(const std::string &name, int x, int y) {
    auto fbo = new FrameBuffer(x, y);
    ResourceManager::getInstance()->addFrameBuffer(name, fbo);
    return fbo;
}