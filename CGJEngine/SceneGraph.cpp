//
// Created by joao on 11/12/17.
//

#include "Shader.h"
#include "OBJMesh.h"
#include "SceneGraph.h"
#include "vec.h"
#include "quat.h"
#include "mat.h"

SceneNode::SceneNode(std::string name) {
    this->name = name;
    mesh = nullptr;
    shader = nullptr;
    position = Vec3(0.0f, 0.0f, 0.0f);
    orientation = Quat(1.0f, 0.0f, 0.0f, 0.0f);
    size = Vec3(1.0f, 1.0f, 1.0f);
    parent = nullptr;
    pre_draw = nullptr;
    post_draw = nullptr;
    visible = true;
}

SceneNode::SceneNode(std::string name, OBJMesh *mesh) {
    this->name = name;
    this->mesh = mesh;
    shader = nullptr;
    position = Vec3(0.0f, 0.0f, 0.0f);
    orientation = Quat(1.0f, 0.0f, 0.0f, 0.0f);
    //orientation = Quat(0, Vec3(0.0f, 1.0f, 0.0f));
    size = Vec3(1.0f, 1.0f, 1.0f);
    parent = nullptr;
    pre_draw = nullptr;
    post_draw = nullptr;
    visible = true;
}

SceneNode::SceneNode(std::string name, OBJMesh *mesh, Shader *shader) {
    this->name = name;
    this->mesh = mesh;
    this->shader = shader;
    position = Vec3(0.0f, 0.0f, 0.0f);
    //orientation = Quat(0, Vec3(0.0f, 1.0f, 0.0f));
    orientation = Quat(1.0f, 0.0f, 0.0f, 0.0f);
    size = Vec3(1.0f, 1.0f, 1.0f);
    parent = nullptr;
    pre_draw = nullptr;
    post_draw = nullptr;
    visible = true;
}

std::string SceneNode::getName() {
    return name;
}

void SceneNode::setMesh(OBJMesh *mesh) {
    this->mesh = mesh;
}

void SceneNode::setShader(Shader *shader) {
    this->shader = shader;
}

void SceneNode::setPosition(float x, float y, float z) {
    position = Vec3(x, y, z);
}

void SceneNode::translate(float x, float y, float z) {
    position = position + Vec3(x, y, z);
}

void SceneNode::setOrientation(float x, float y, float z, float angle) {
    orientation = Quat(angle, Vec3(x, y, z));
}

void SceneNode::setOrientation(Quat quat) {
    orientation = quat;
}

void SceneNode::rotate(float x, float y, float z, float angle) {
    orientation = Quat(angle, Vec3(x, y, z)) * orientation;
}

void SceneNode::scale(float x, float y, float z) {
    size = Vec3(x, y, z);
}

void SceneNode::setPreDraw(std::function<void()> callback) {
    pre_draw = callback;
}

void SceneNode::setPostDraw(std::function<void()> callback) {
    post_draw = callback;
}

void SceneNode::addChild(SceneNode *child) {
    child->parent = this;
    childs.push_back(child);
}

void SceneNode::destroy() {
    //Destroy all childs
    for (auto it = childs.begin(); it != childs.end(); ) {
        (*it)->destroy();
        delete (*it);
        it = childs.erase(it);
    }
}

void SceneNode::update(int dt) {
    //Empty for now
    for(SceneNode* n : childs)
        n->update(dt);
}

Mat4 SceneNode::getTranslation() {
    if(parent == nullptr)
        return CGJM::translate(position[0], position[1], position[2]);
    else
        return CGJM::translate(position[0], position[1], position[2]) *
               parent->getTranslation();
}

Mat4 SceneNode::getScale() {
    if(parent == nullptr)
        return CGJM::scale(size[0], size[1], size[2]);
    else
        return CGJM::scale(size[0], size[1], size[2])*
               parent->getScale();
}

Quat SceneNode::getOrientation() {
    if(parent == nullptr)
        return orientation;
    else
        return orientation * parent->getOrientation();
}

Mat4 SceneNode::getModelMatrix() {
        return getTranslation()*getOrientation().GLMatrix().transpose()*getScale();
}

void SceneNode::hidden(bool b) {
    visible = !b;
}

SceneNode* SceneNode::findNode(std::string &name) {
    if(name == this->name)
        return this;
    for(SceneNode* n: childs){
        SceneNode* found = n->findNode(name);
        if(found != nullptr)
            return found;
    }
    return nullptr;
}

void SceneNode::draw(SceneGraph *scene) {
    if(!visible)
        return;

    if(mesh != nullptr) {
        //Set the shader
        if (shader != nullptr)
            shader->use();
        else {
            SceneNode *n = this;
            while (n->parent != nullptr) {
                if (n->parent->shader != nullptr) {
                    n->parent->shader->use();
                    break;
                } else {
                    if (parent == nullptr) {
                        std::cerr << "Missing shader in Scenegraph" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    n = parent;
                }
            }
        }
    }
    //Call pre draw
    if(pre_draw != nullptr)
        pre_draw();

    if(mesh != nullptr) {
        //Upload MVP
        Mat4 P = scene->getProjectionMatrix();
        Mat4 V = scene->getViewMatrix();
        Mat4 M = getModelMatrix();
        shader->uploadMVP(M, V, P);

        //Draw
        mesh->draw();
    }
    //Call post draw
    if(post_draw != nullptr)
        post_draw();

    glUseProgram(0);

    //Draw the childs
    for(SceneNode* n : childs)
        n->draw(scene);
}

SceneGraph::SceneGraph() {
    camera = nullptr;
    root = nullptr;
}

SceneGraph::SceneGraph(Camera *cam) {
    camera = cam;
    root = nullptr;
}

SceneGraph::SceneGraph(Camera *cam, SceneNode *rootNode) {
    camera = cam;
    root = rootNode;
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
        root->draw(this);
}

SceneNode* SceneGraph::findNode(std::string& name) {
    return root->findNode(name);
}