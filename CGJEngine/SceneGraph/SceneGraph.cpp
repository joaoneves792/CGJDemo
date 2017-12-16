//
// Created by joao on 11/12/17.
//
#include <iostream>

#include "Shader.h"
#include "SceneGraph/SceneGraph.h"
#include "SceneGraph/SceneNode.h"
#include "glm_wrapper.h"

SceneGraph::SceneGraph(Camera *cam) {
    _camera = cam;
    _root = new SceneNode(ROOT);
    _lookUpTable[ROOT] = _root;
    _root->setScene(this);
}

void SceneGraph::destroy() {
    if(_root != nullptr) {
        _root->destroy();
        auto it = _lookUpTable.find(ROOT);
        _lookUpTable.erase(it);
        delete _root;
    }
    _lookUpTable.clear();
}

Mat4 SceneGraph::getProjectionMatrix() {
    if(_camera != nullptr)
        return _camera->getProjectionMatrix();
    else
        return Mat4(1.0f);
}

Mat4 SceneGraph::getViewMatrix() {
    if(_camera != nullptr)
        return _camera->getViewMatrix();
    else
        return Mat4(1.0f);
}

Camera* SceneGraph::getCamera() {
    return _camera;
}

SceneNode* SceneGraph::getRoot() {
    return _root;
}

void SceneGraph::update(int dt) {
    if(_root != nullptr)
        _root->update(dt);
}

void SceneGraph::draw(int level){
    if(_root != nullptr)
        _root->draw(level, Mat4(1.0f), Quat(), Mat4(1.0f));
}

void SceneGraph::draw(){
    draw(0);
}
SceneNode* SceneGraph::findNode(const std::string& name) {
    auto it = _lookUpTable.find(name);
    if(it!=_lookUpTable.end())
        return it->second;
    else{
        std::cerr << "Node with name " << name << " not on scene graph!" << std::endl;
        return nullptr;
    }

}