//
// Created by joao on 11/15/17.
//

#include "SceneGraph.h"
#include "Shader.h"
#include "Mesh.h"
#include "ResourceManager.h"
#include <unordered_map>

ResourceManager* ResourceManager::_ourInstance = nullptr;

ResourceManager* ResourceManager::getInstance() {
    if(_ourInstance == nullptr){
        _ourInstance = new ResourceManager();
    }
    return _ourInstance;
}

void ResourceManager::deleteInstance() {
    delete _ourInstance;
    _ourInstance = nullptr;
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
    _meshes[name] = mesh;
}

void ResourceManager::addShader(std::string name, Shader *shader) {
    _shaders[name] = shader;
}

void ResourceManager::addScene(std::string name, SceneGraph *scene) {
    _scenes[name] = scene;
}

void ResourceManager::addCamera(std::string name, Camera *camera) {
    _cameras[name] = camera;
}

void ResourceManager::addFrameBuffer(std::string name, FrameBuffer *fbo) {
    _fbos[name] = fbo;
}

Mesh* ResourceManager::getMesh(std::string name) {
    auto it = _meshes.find(name);
    if(it == _meshes.end()){
        return nullptr;
    }
    return it->second;
}

Shader* ResourceManager::getShader(std::string name) {
    auto it = _shaders.find(name);
    if(it == _shaders.end()){
        return nullptr;
    }
    return it->second;
}

SceneGraph* ResourceManager::getScene(std::string name) {
    auto it = _scenes.find(name);
    if(it == _scenes.end()){
        return nullptr;
    }
    return it->second;
}

Camera* ResourceManager::getCamera(std::string name){
    auto it = _cameras.find(name);
    if(it == _cameras.end()){
        return nullptr;
    }
    return it->second;
}

FrameBuffer* ResourceManager::getFrameBuffer(std::string name) {
    auto it = _fbos.find(name);
    if(it == _fbos.end()){
        return nullptr;
    }
    return it->second;
}

void ResourceManager::destroyMesh(std::string name) {
    auto it = _meshes.find(name);
    if(it != _meshes.end()){
        Mesh* m = it->second;
        __destroyMesh(m);
        _meshes.erase(it);
    }
}

void ResourceManager::destroyShader(std::string name) {
    auto it = _shaders.find(name);
    if(it != _shaders.end()){
        Shader* s = it->second;
        __destroyShader(s);
        _shaders.erase(it);
    }
}

void ResourceManager::destroyScene(std::string name) {
    auto it = _scenes.find(name);
    if(it != _scenes.end()){
        SceneGraph* s = it->second;
        __destroyScene(s);
        _scenes.erase(it);
    }
}

void ResourceManager::destroyCamera(std::string name) {
    auto it = _cameras.find(name);
    if(it != _cameras.end()){
        __destroyCamera(it->second);
        _cameras.erase(it);
    }
}

void ResourceManager::destroyFrameBuffer(std::string name) {
    auto it = _fbos.find(name);
    if(it != _fbos.end()){
        __destroyFrameBuffer(it->second);
        _fbos.erase(it);
    }
}

void ResourceManager::destroyAllMeshes() {
    for(auto it=_meshes.begin(); it!=_meshes.end(); it++){
        __destroyMesh(it->second);
    }
    _meshes.clear();
}

void ResourceManager::destroyAllShaders() {
    for(auto it=_shaders.begin(); it!=_shaders.end(); it++){
        __destroyShader(it->second);
    }
    _shaders.clear();
}

void ResourceManager::destroyAllScenes() {
    for(auto it=_scenes.begin(); it!=_scenes.end(); it++){
        __destroyScene(it->second);
    }
    _scenes.clear();
}

void ResourceManager::destroyAllCameras() {
    for(auto it=_cameras.begin(); it!=_cameras.end(); it++){
        __destroyCamera(it->second);
    }
    _cameras.clear();
}

void ResourceManager::destroyAllFrameBuffers() {
    for(auto it=_fbos.begin(); it!=_fbos.end(); it++){
        __destroyFrameBuffer(it->second);
    }
    _fbos.clear();
}

void ResourceManager::destroyEverything() {
    destroyAllMeshes();
    destroyAllShaders();
    destroyAllScenes();
    destroyAllCameras();
    destroyAllFrameBuffers();
}

