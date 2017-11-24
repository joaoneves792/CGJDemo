//
// Created by joao on 11/15/17.
//

#include "SceneGraph.h"
#include "Shader.h"
#include "Mesh.h"
#include "ResourceManager.h"
#include <unordered_map>

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

void ResourceManager::__destroyMesh(Mesh *mesh) {
    mesh->unload();
    delete mesh;
}

void ResourceManager::__destroyShader(Shader *shader) {
    delete shader;
}

void ResourceManager::__destroyScene(SceneGraph *scene) {
    scene->destroy();
    delete scene;
}

void ResourceManager::__destroyCamera(Camera *camera) {
    delete camera;
}

void ResourceManager::__destroyFrameBuffer(FrameBuffer *fbo) {
    fbo->unbind();
    delete fbo;
}

void ResourceManager::addMesh(std::string name, Mesh *mesh) {
    meshes[name] = mesh;
}

void ResourceManager::addShader(std::string name, Shader *shader) {
    shaders[name] = shader;
}

void ResourceManager::addScene(std::string name, SceneGraph *scene) {
    scenes[name] = scene;
}

void ResourceManager::addCamera(std::string name, Camera *camera) {
    cameras[name] = camera;
}

void ResourceManager::addFrameBuffer(std::string name, FrameBuffer *fbo) {
    fbos[name] = fbo;
}

Mesh* ResourceManager::getMesh(std::string name) {
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

Camera* ResourceManager::getCamera(std::string name){
    auto it = cameras.find(name);
    if(it == cameras.end()){
        return nullptr;
    }
    return it->second;
}

FrameBuffer* ResourceManager::getFrameBuffer(std::string name) {
    auto it = fbos.find(name);
    if(it == fbos.end()){
        return nullptr;
    }
    return it->second;
}

void ResourceManager::destroyMesh(std::string name) {
    auto it = meshes.find(name);
    if(it != meshes.end()){
        Mesh* m = it->second;
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

void ResourceManager::destroyCamera(std::string name) {
    auto it = cameras.find(name);
    if(it != cameras.end()){
        __destroyCamera(it->second);
        cameras.erase(it);
    }
}

void ResourceManager::destroyFrameBuffer(std::string name) {
    auto it = fbos.find(name);
    if(it != fbos.end()){
        __destroyFrameBuffer(it->second);
        fbos.erase(it);
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

void ResourceManager::destroyAllCameras() {
    for(auto it=cameras.begin(); it!=cameras.end(); it++){
        __destroyCamera(it->second);
    }
    cameras.clear();
}

void ResourceManager::destroyAllFrameBuffers() {
    for(auto it=fbos.begin(); it!=fbos.end(); it++){
        __destroyFrameBuffer(it->second);
    }
    fbos.clear();
}

void ResourceManager::destroyEverything() {
    destroyAllMeshes();
    destroyAllShaders();
    destroyAllScenes();
    destroyAllCameras();
    destroyAllFrameBuffers();
}

