//
// Created by joao on 11/23/17.
//

#ifndef CGJDEMO_SCENENODE_H
#define CGJDEMO_SCENENODE_H

#include <functional>
#include <list>
#include <string>

#include "glm_wrapper.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"

class SceneGraph;

class SceneNode{
protected:
    SceneGraph* _scene;

    std::string _name;
    Mesh* _mesh;
    Shader* _shader;

    bool _billboard;
    Vec3 _position;
    Quat _orientation;
    Vec3 _size;

    bool _visible;

    SceneNode* _parent;
    std::list<SceneNode*> _childs = {};

    std::function<void()> _preDraw;
    std::function<void()> _postDraw;
    std::function<void(int dt)> _updateCallback;

    int _processingLevel = 0;

public:
    SceneNode(std::string name);
    SceneNode(std::string name, Mesh* mesh);
    SceneNode(std::string name, Mesh* mesh, Shader* shader);

    virtual ~SceneNode() = default;

    std::string getName();

    void setMesh(Mesh* mesh);
    void setShader(Shader* shader);

    void setScene(SceneGraph* sceneGraph);

    void setProcessingLevel(int level);

    void setPosition(float x, float y, float z);
    void translate(float x, float y, float z);
    virtual Vec3 getPosition();

    void setOrientation(float x, float y, float z, float angle);
    void setOrientation(Quat quat);
    void rotate(float x, float y, float z, float angle);
    void scale(float x, float y, float z);

    void setPreDraw(std::function<void()> callback);
    void setPostDraw(std::function<void()> callback);
    void setUpdateCallback(std::function<void(int dt)> callback);

    void addChild(SceneNode* child);
    void destroy();

    void hidden(bool b);
    void setBillboard(bool billboarded);


    void update(int dt);
    void draw(int level);

    Mat4 getModelMatrix();


    SceneNode* findNode(const std::string& name);

protected:
    SceneGraph* getScene();
    Mat4 billboardMatrix(Mat4 View);
    Mat4 getTranslation();
    Quat getOrientation();
    Mat4 getScale();
};
#endif //CGJDEMO_SCENENODE_H