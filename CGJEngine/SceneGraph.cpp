//
// Created by joao on 11/12/17.
//

#include "Shader.h"
#include "SceneGraph.h"
#include "SceneNode.h"

SceneGraph::SceneGraph(Camera *cam) {
    camera = cam;
    root = new SceneNode(ROOT);
    lookUpTable[ROOT] = root;
    root->setScene(this);
}

void SceneGraph::destroy() {
    if(root != nullptr) {
        root->destroy();
        auto it = lookUpTable.find(ROOT);
        lookUpTable.erase(it);
        delete root;
    }
    lookUpTable.clear();
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
    auto it = lookUpTable.find(name);
    if(it!=lookUpTable.end())
        return it->second;
    else{
        std::cerr << "Node with name " << name << " not on scene graph!" << std::endl;
        return nullptr;
    }

}