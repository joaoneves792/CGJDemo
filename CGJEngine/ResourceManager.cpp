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

void ResourceManager::__destroyParticlePool(ParticlePool *pool) {
    delete pool;
}

void ResourceManager::addMesh(const std::string& name, Mesh *mesh) {
    _meshes[name] = mesh;
}

void ResourceManager::addShader(const std::string& name, Shader *shader) {
    _shaders[name] = shader;
}

void ResourceManager::addScene(const std::string& name, SceneGraph *scene) {
    _scenes[name] = scene;
}

void ResourceManager::addCamera(const std::string& name, Camera *camera) {
    _cameras[name] = camera;
}

void ResourceManager::addFrameBuffer(const std::string& name, FrameBuffer *fbo) {
    _fbos[name] = fbo;
}

void ResourceManager::addParticlePool(const std::string &name, ParticlePool *pool) {
    _pools[name] = pool;
}

Mesh* ResourceManager::getMesh(const std::string& name) {
    auto it = _meshes.find(name);
    if(it == _meshes.end()){
        return nullptr;
    }
    return it->second;
}

Shader* ResourceManager::getShader(const std::string& name) {
    auto it = _shaders.find(name);
    if(it == _shaders.end()){
        return nullptr;
    }
    return it->second;
}

SceneGraph* ResourceManager::getScene(const std::string& name) {
    auto it = _scenes.find(name);
    if(it == _scenes.end()){
        return nullptr;
    }
    return it->second;
}

Camera* ResourceManager::getCamera(const std::string& name){
    auto it = _cameras.find(name);
    if(it == _cameras.end()){
        return nullptr;
    }
    return it->second;
}

FrameBuffer* ResourceManager::getFrameBuffer(const std::string& name) {
    auto it = _fbos.find(name);
    if(it == _fbos.end()){
        return nullptr;
    }
    return it->second;
}

ParticlePool* ResourceManager::getParticlePool(const std::string &name) {
    auto it = _pools.find(name);
    if(it == _pools.end()){
        return nullptr;
    }
    return it->second;
}

void ResourceManager::destroyMesh(const std::string& name) {
    auto it = _meshes.find(name);
    if(it != _meshes.end()){
        Mesh* m = it->second;
        __destroyMesh(m);
        _meshes.erase(it);
    }
}

void ResourceManager::destroyShader(const std::string& name) {
    auto it = _shaders.find(name);
    if(it != _shaders.end()){
        Shader* s = it->second;
        __destroyShader(s);
        _shaders.erase(it);
    }
}

void ResourceManager::destroyScene(const std::string& name) {
    auto it = _scenes.find(name);
    if(it != _scenes.end()){
        SceneGraph* s = it->second;
        __destroyScene(s);
        _scenes.erase(it);
    }
}

void ResourceManager::destroyCamera(const std::string& name) {
    auto it = _cameras.find(name);
    if(it != _cameras.end()){
        __destroyCamera(it->second);
        _cameras.erase(it);
    }
}

void ResourceManager::destroyFrameBuffer(const std::string& name) {
    auto it = _fbos.find(name);
    if(it != _fbos.end()){
        __destroyFrameBuffer(it->second);
        _fbos.erase(it);
    }
}

void ResourceManager::destroyParticlePool(const std::string &name) {
    auto it = _pools.find(name);
    if(it != _pools.end()){
        __destroyParticlePool(it->second);
        _pools.erase(it);
    }
}

void ResourceManager::destroyAllMeshes() {
    for(auto it: _meshes){
        __destroyMesh(it.second);
    }
    _meshes.clear();
}

void ResourceManager::destroyAllShaders() {
    for(auto it: _shaders){
        __destroyShader(it.second);
    }
    _shaders.clear();
}

void ResourceManager::destroyAllScenes() {
    for(auto it:_scenes){
        __destroyScene(it.second);
    }
    _scenes.clear();
}

void ResourceManager::destroyAllCameras() {
    for(auto it : _cameras){
        __destroyCamera(it.second);
    }
    _cameras.clear();
}

void ResourceManager::destroyAllFrameBuffers() {
    for(auto it: _fbos){
        __destroyFrameBuffer(it.second);
    }
    _fbos.clear();
}

void ResourceManager::destroyAllParticlePools() {
    for(auto it : _pools){
        __destroyParticlePool(it.second);
    }
}

void ResourceManager::destroyEverything() {
    destroyAllMeshes();
    destroyAllShaders();
    destroyAllScenes();
    destroyAllCameras();
    destroyAllFrameBuffers();
    destroyAllParticlePools();
}

