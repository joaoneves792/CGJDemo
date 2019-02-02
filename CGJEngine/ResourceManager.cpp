//
// Created by joao on 11/15/17.
//

#include "GL/glew.h"
#include "SceneGraph/SceneGraph.h"
#include "Shader.h"
#include "Meshes/Mesh.h"
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

void ResourceManager::__destroyTexture(std::shared_ptr<Texture>& texture) {
    if(texture.unique())
        texture->destroyTexture();
    texture.reset();
}

void ResourceManager::__destroyNoise(Noise *fur) {
    delete fur;
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

void ResourceManager::addTexture(const std::string &name, std::shared_ptr<Texture> texture) {
    _textures[name] = texture;
}

void ResourceManager::addNoise(const std::string &name, Noise *fur) {
    _noise[name] = fur;
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

std::shared_ptr<Texture> ResourceManager::getTexture(const std::string &name) {
    auto it = _textures.find(name);
    if(it == _textures.end()){
        return nullptr;
    }
    return it->second;
}

Noise* ResourceManager::getNoise(const std::string &name) {
    auto it = _noise.find(name);
    if(it == _noise.end()){
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

void ResourceManager::destroyTexture(const std::string &name) {
    auto it = _textures.find(name);
    if(it != _textures.end()){
        __destroyTexture(it->second);
        _textures.erase(it);
    }
}

void ResourceManager::destroyNoise(const std::string &name) {
    auto it = _noise.find(name);
    if(it != _noise.end()){
        __destroyNoise(it->second);
        _noise.erase(it);
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
    _pools.clear();
}

void ResourceManager::destroyAllTextures() {
    for(auto it : _textures){
        __destroyTexture(it.second);
    }
    _textures.clear();
}

void ResourceManager::destroyAllNoise() {
    for(auto it : _noise){
        __destroyNoise(it.second);
    }
    _noise.clear();
}

void ResourceManager::destroyEverything() {
    destroyAllMeshes();
    destroyAllTextures(); //Meshes can have textures, so destroy Textures only after meshes
    destroyAllNoise();
    destroyAllShaders();
    destroyAllScenes();
    destroyAllCameras();
    destroyAllFrameBuffers();
    destroyAllParticlePools();
}

