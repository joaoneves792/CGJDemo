//
// Created by joao on 11/12/17.
//

#include "Shader.h"
#include "SceneGraph.h"
#include "SceneNode.h"

SceneGraph::SceneGraph(Camera *cam) {
    camera = cam;
    root = new SceneNode("root");
    root->setScene(this);
}

void SceneGraph::destroy() {
    if(root != nullptr) {
        root->destroy();
        delete root;
    }
}

Mat4 SceneGraph::getProjectionMatrix() {
    if(camera != nullptr)
        return camera->getProjectionMatrix();
    else
        return Mat4(1);
}

Mat4 SceneGraph::getViewMatrix() {
    if(camera != nullptr)
        return camera->getViewMatrix();
    else
        return Mat4(1);
}

Camera* SceneGraph::getCamera() {
    return camera;
}

SceneNode* SceneGraph::getRoot() {
    return root;
}

void SceneGraph::update(int dt) {
    if(root != nullptr)
        root->update(dt);
}

void SceneGraph::draw(){
    if(root != nullptr)
        root->draw();
}

SceneNode* SceneGraph::findNode(const std::string& name) {
    return root->findNode(name);
}