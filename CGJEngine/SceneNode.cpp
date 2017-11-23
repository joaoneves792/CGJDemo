//
// Created by joao on 11/23/17.
//

#include "SceneGraph.h"
#include "Mesh.h"
#include "Shader.h"
#include "SceneNode.h"

SceneNode::SceneNode(std::__cxx11::string name) {
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
    scene = nullptr;
    billboard = false;
}

SceneNode::SceneNode(std::__cxx11::string name, Mesh *mesh) {
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
    scene = nullptr;
    billboard = false;
}

SceneNode::SceneNode(std::__cxx11::string name, Mesh *mesh, Shader *shader) {
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
    scene = nullptr;
    billboard = false;
}

std::__cxx11::string SceneNode::getName() {
    return name;
}

void SceneNode::setMesh(Mesh *mesh) {
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

Vec3 SceneNode::getPosition(){
    return position;
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

void SceneNode::setUpdateCallback(std::function<void(int dt)> callback) {
    updateCallback = callback;
}

void SceneNode::setScene(SceneGraph *sceneGraph) {
    scene = sceneGraph;
}

SceneGraph* SceneNode::getScene(){
    if(scene != nullptr)
        return scene;
    //If we dont have a scene then somebody upstream must have it
    if(parent != nullptr) {
        scene = parent->getScene();
        return scene;
    }else {
        std::cerr << "Warning: Scene node " << name << " has no scene and no parent!";
        return nullptr; //This should not happen!
    }
}

void SceneNode::addChild(SceneNode *child) {
    child->parent = this;
    child->setScene(scene);
    childs.push_back(child);
    scene->lookUpTable[child->name] = child;
}

void SceneNode::destroy() {
    //Destroy all childs
    for (auto it = childs.begin(); it != childs.end(); ) {

        getScene();
        auto lookUpIt = scene->lookUpTable.find((*it)->name);
        if(lookUpIt != scene->lookUpTable.end())
            scene->lookUpTable.erase(lookUpIt);

        (*it)->destroy();
        delete (*it);
        it = childs.erase(it);
    }
}

void SceneNode::update(int dt) {
    if(scene == nullptr)
        scene = getScene();

    if(updateCallback != nullptr)
        updateCallback(dt);

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
        return CGJM::scale(size[0], size[1], size[2]) *
               parent->getScale();
}

Quat SceneNode::getOrientation() {
    Quat result = orientation;
    if (billboard) {
        //Invert the rotation of The view Matrix (simply the transpose of the rotation part)
        Mat4 View = scene->getCamera()->getViewMatrix();
        Mat4 ViewT = Mat4(View[0][0], View[1][0], View[2][0], 0,
                          View[0][1], View[1][1], View[2][1], 0,
                          View[0][2], View[1][2], View[2][2], 0,
                          0, 0, 0, 1);
        //Convert it into a quat
        result = rotationMatrixToQuat(ViewT);
    }
    if(parent == nullptr)
        return result;
    else
        return result * parent->getOrientation();
}

Mat4 SceneNode::getModelMatrix() {
        return getTranslation()*getOrientation().GLMatrix().transpose()*getScale();
}

Mat4 SceneNode::billboardMatrix(Mat4 View) {
    return Mat4(View[0][0], View[1][0], View[2][0], 0,
                View[0][1], View[1][1], View[2][1], 0,
                View[0][2], View[1][2], View[2][2], 0,
                0, 0, 0, 1);
}

void SceneNode::setBillboard(bool billboarded) {
    billboard = billboarded;
}

void SceneNode::hidden(bool b) {
    visible = !b;
}

SceneNode* SceneNode::findNode(const std::string &name) {
    auto it = scene->lookUpTable.find(name);
    if(it!=scene->lookUpTable.end())
        return it->second;

    std::cerr << "Node with name " << name << "not found in scenegraph."<< std::endl;
    return nullptr;
}

void SceneNode::draw() {
    if(!visible)
        return;

    if(scene == nullptr)
        scene = getScene();

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
        n->draw();
}