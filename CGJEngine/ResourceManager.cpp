//
// Created by joao on 11/15/17.
//

#include "SceneGraph.h"
#include "Shader.h"
#include "OBJMesh.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::ourInstance = nullptr;

ResourceManager* ResourceManager::getInstance() {
    if(ourInstance == nullptr){
        ourInstance = new ResourceManager();
    }
    return ourInstance;
}

void ResourceManager::deleteInstance() {
    delete ourInstance;
    ourInstance = nullptr;
}

void ResourceManager::__destroyMesh(OBJMesh *mesh) {
    mesh->unload();
    delete mesh;
}

void ResourceManager::__destroyShader(Shader *shader) {
    delete shader;
}

void ResourceManager::__destroyScene(SceneGraph *scene) {
    delete scene->getCamera();
    scene->destroy();
    delete scene;
}

void ResourceManager::addMesh(std::string name, OBJMesh *mesh) {
    meshes[name] = mesh;
}

void ResourceManager::addShader(std::string name, Shader *shader) {
    shaders[name] = shader;
}

void ResourceManager::addScene(std::string name, SceneGraph *scene) {
    scenes[name] = scene;
}

OBJMesh* ResourceManager::getMesh(std::string name) {
    auto it = meshes.find(name);
    if(it == meshes.end()){
        return nullptr;
    }
    return it->second;
}

Shader* ResourceManager::getShader(std::string name) {
    auto it = shaders.find(name);
    if(it == shaders.end()){
        return nullptr;
    }
    return it->second;
}

SceneGraph* ResourceManager::getScene(std::string name) {
    auto it = scenes.find(name);
    if(it == scenes.end()){
        return nullptr;
    }
    return it->second;
}

void ResourceManager::destroyMesh(std::string name) {
    auto it = meshes.find(name);
    if(it != meshes.end()){
        OBJMesh* m = it->second;
        __destroyMesh(m);
        meshes.erase(it);
    }
}

void ResourceManager::destroyShader(std::string name) {
    auto it = shaders.find(name);
    if(it != shaders.end()){
        Shader* s = it->second;
        __destroyShader(s);
        shaders.erase(it);
    }
}

void ResourceManager::destroyScene(std::string name) {
    auto it = scenes.find(name);
    if(it != scenes.end()){
        SceneGraph* s = it->second;
        __destroyScene(s);
        scenes.erase(it);
    }
}

void ResourceManager::destroyAllMeshes() {
    for(auto it=meshes.begin(); it!=meshes.end(); it++){
        __destroyMesh(it->second);
    }
    meshes.clear();
}

void ResourceManager::destroyAllShaders() {
    for(auto it=shaders.begin(); it!=shaders.end(); it++){
        __destroyShader(it->second);
    }
    shaders.clear();
}

void ResourceManager::destroyAllScenes() {
    for(auto it=scenes.begin(); it!=scenes.end(); it++){
        __destroyScene(it->second);
    }
    scenes.clear();
}

void ResourceManager::destroyEverything() {
    destroyAllMeshes();
    destroyAllShaders();
    destroyAllScenes();
}
