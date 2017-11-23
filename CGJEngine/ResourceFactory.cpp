//
// Created by joao on 11/21/17.
//

#include "ResourceFactory.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "H3DMesh.h"
#include "Shader.h"
#include "SceneGraph.h"
#include "LightNode.h"
#include "ResourceManager.h"
#include "LightsManager.h"

Mesh* ResourceFactory::createMesh(const std::string &name, const std::string &filename) {
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

Shader* ResourceFactory::createShader(const std::string &name, const std::string &vertexShader,
                                      const std::string fragmentShader) {
    auto shader = new Shader(vertexShader.c_str(), fragmentShader.c_str());
    ResourceManager::getInstance()->addShader(name, shader);
    return shader;
}

SceneNode* ResourceFactory::createScene(const std::string &name, Camera *camera) {
    auto scene = new SceneGraph(camera);
    ResourceManager::getInstance()->addScene(name, scene);
    return scene->getRoot();
}

LightNode* ResourceFactory::createLight(const std::string &name) {
    auto light = new LightNode(name);
    LightsManager::getInstance()->addLight(light);
    return light;
}
