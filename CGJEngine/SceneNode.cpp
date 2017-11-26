//
// Created by joao on 11/23/17.
//

#include <iostream>
#include "SceneGraph.h"
#include "Mesh.h"
#include "Shader.h"
#include "SceneNode.h"
#include "glm_wrapper.h"

SceneNode::SceneNode(std::string name) {
    this->_name = name;
    _mesh = nullptr;
    _shader = nullptr;
    _position = Vec3(0.0f, 0.0f, 0.0f);
    _orientation = Quat();
    _size = Vec3(1.0f, 1.0f, 1.0f);
    _parent = nullptr;
    _preDraw = nullptr;
    _postDraw = nullptr;
    _visible = true;
    _scene = nullptr;
    _billboard = false;
}

SceneNode::SceneNode(std::string name, Mesh *mesh) {
    this->_name = name;
    this->_mesh = mesh;
    _shader = nullptr;
    _position = Vec3(0.0f, 0.0f, 0.0f);
    _orientation = Quat();
    _size = Vec3(1.0f, 1.0f, 1.0f);
    _parent = nullptr;
    _preDraw = nullptr;
    _postDraw = nullptr;
    _visible = true;
    _scene = nullptr;
    _billboard = false;
}

SceneNode::SceneNode(std::string name, Mesh *mesh, Shader *shader) {
    this->_name = name;
    this->_mesh = mesh;
    this->_shader = shader;
    _position = Vec3(0.0f, 0.0f, 0.0f);
    _orientation = Quat();
    _size = Vec3(1.0f, 1.0f, 1.0f);
    _parent = nullptr;
    _preDraw = nullptr;
    _postDraw = nullptr;
    _visible = true;
    _scene = nullptr;
    _billboard = false;
}

std::string SceneNode::getName() {
    return _name;
}

void SceneNode::setMesh(Mesh *mesh) {
    this->_mesh = mesh;
}

void SceneNode::setShader(Shader *shader) {
    this->_shader = shader;
}

void SceneNode::setProcessingLevel(int level) {
    _processingLevel = level;
}

void SceneNode::setPosition(float x, float y, float z) {
    _position = Vec3(x, y, z);
}

void SceneNode::translate(float x, float y, float z) {
    _position = _position + Vec3(x, y, z);
}

Vec3 SceneNode::getPosition(){
    return _position;
}

void SceneNode::setOrientation(float x, float y, float z, float angle) {
    _orientation = glm::angleAxis(angle, Vec3(x, y, z));
}

void SceneNode::setOrientation(Quat quat) {
    _orientation = quat;
}

void SceneNode::rotate(float x, float y, float z, float angle) {
    _orientation = glm::angleAxis(angle, Vec3(x, y, z)) * _orientation;
}

void SceneNode::scale(float x, float y, float z) {
    _size = Vec3(x, y, z);
}

void SceneNode::setPreDraw(std::function<void()> callback) {
    _preDraw = callback;
}

void SceneNode::setPostDraw(std::function<void()> callback) {
    _postDraw = callback;
}

void SceneNode::setUpdateCallback(std::function<void(int dt)> callback) {
    _updateCallback = callback;
}

void SceneNode::setScene(SceneGraph *sceneGraph) {
    _scene = sceneGraph;
}

SceneGraph* SceneNode::getScene(){
    if(_scene != nullptr)
        return _scene;
    //If we dont have a scene then somebody upstream must have it
    if(_parent != nullptr) {
        _scene = _parent->getScene();
        return _scene;
    }else {
        std::cerr << "Warning: Scene node " << _name << " has no scene and no parent!";
        return nullptr; //This should not happen!
    }
}

void SceneNode::addChild(SceneNode *child) {
    child->_parent = this;
    child->setScene(_scene);
    _childs.push_back(child);
    _scene->_lookUpTable[child->_name] = child;
}

void SceneNode::destroy() {
    //Destroy all childs
    for (auto it = _childs.begin(); it != _childs.end(); ) {

        getScene();
        auto lookUpIt = _scene->_lookUpTable.find((*it)->_name);
        if(lookUpIt != _scene->_lookUpTable.end())
            _scene->_lookUpTable.erase(lookUpIt);

        (*it)->destroy();
        delete (*it);
        it = _childs.erase(it);
    }
}

void SceneNode::update(int dt) {
    if(_scene == nullptr)
        _scene = getScene();

    if(_updateCallback != nullptr)
        _updateCallback(dt);

    for(SceneNode* n : _childs)
        n->update(dt);
}

Mat4 SceneNode::getTranslation() {
    if(_parent == nullptr)
        return glm::translate(Mat4(1.0f), _position);
    else
        return glm::translate(Mat4(1.0f), _position) *
               _parent->getTranslation();
}

Mat4 SceneNode::getScale() {
    if(_parent == nullptr)
        return glm::scale(Mat4(1.0f), _size);
    else
        return glm::scale(Mat4(1.0f), _size) *
               _parent->getScale();
}

Quat SceneNode::getOrientation() {
    Quat result = _orientation;
    if (_billboard) {
        //Invert the rotation of The view Matrix (simply the transpose of the rotation part)
        Mat4 View = _scene->getCamera()->getViewMatrix();
        Mat4 ViewT = Mat4(View[0][0], View[1][0], View[2][0], 0,
                          View[0][1], View[1][1], View[2][1], 0,
                          View[0][2], View[1][2], View[2][2], 0,
                          0, 0, 0, 1);
        //Convert it into a quat
        result = glm::toQuat(ViewT);
    }
    if(_parent == nullptr)
        return result;
    else
        return result * _parent->getOrientation();
}

Mat4 SceneNode::getModelMatrix() {
        return getTranslation()*glm::toMat4(getOrientation())*getScale();
}

Mat4 SceneNode::billboardMatrix(Mat4 View) {
    return Mat4(View[0][0], View[1][0], View[2][0], 0,
                View[0][1], View[1][1], View[2][1], 0,
                View[0][2], View[1][2], View[2][2], 0,
                0, 0, 0, 1);
}

void SceneNode::setBillboard(bool billboarded) {
    _billboard = billboarded;
}

void SceneNode::hidden(bool b) {
    _visible = !b;
}

SceneNode* SceneNode::findNode(const std::string &name) {
    auto it = _scene->_lookUpTable.find(name);
    if(it!=_scene->_lookUpTable.end())
        return it->second;

    std::cerr << "Node with name " << name << "not found in scenegraph."<< std::endl;
    return nullptr;
}

void SceneNode::draw(int level) {
    if(!_visible)
        return;

    if(level == _processingLevel) {

        if (_scene == nullptr)
            _scene = getScene();

        if (_mesh != nullptr) {
            //Set the shader
            if (_shader != nullptr)
                _shader->use();
            else {
                SceneNode *n = this;
                while (n->_parent != nullptr) {
                    if (n->_parent->_shader != nullptr) {
                        n->_parent->_shader->use();
                        break;
                    } else {
                        if (_parent == nullptr) {
                            std::cerr << "Missing shader in Scenegraph" << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        n = _parent;
                    }
                }
            }
        }
        //Call pre draw
        if (_preDraw != nullptr)
            _preDraw();

        if (_mesh != nullptr) {
            //Upload MVP
            Mat4 P = _scene->getProjectionMatrix();
            Mat4 V = _scene->getViewMatrix();
            Mat4 M = getModelMatrix();
            _shader->uploadMVP(M, V, P);

            //Draw
            _mesh->draw();
        }
        //Call post draw
        if (_postDraw != nullptr)
            _postDraw();

        glUseProgram(0);
    }
    //Draw the childs
    for(SceneNode* n : _childs)
        n->draw(level);
}