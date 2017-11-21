//
// Created by joao on 11/12/17.
//

#ifndef CGJM_SCENEGRAPH_H
#define CGJM_SCENEGRAPH_H

#include <functional>
#include <list>
#include <string>

#include "vec.h"
#include "mat.h"
#include "quat.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"

class SceneGraph;

class SceneNode{
private:
    SceneGraph* scene;

    std::string name;
    Mesh* mesh;
    Shader* shader;

    bool billboard;
    Vec3 position;
    Quat orientation;
    Vec3 size;

    bool visible;

    SceneNode* parent;
    std::list<SceneNode*> childs = {};

    std::function<void()> pre_draw;
    std::function<void()> post_draw;
    std::function<void(int dt)> updateCallback;

public:
    SceneNode(std::string name);
    SceneNode(std::string name, Mesh* mesh);
    SceneNode(std::string name, Mesh* mesh, Shader* shader);

    std::string getName();

    void setMesh(Mesh* mesh);
    void setShader(Shader* shader);

    void setScene(SceneGraph* sceneGraph);

    void setPosition(float x, float y, float z);
    void translate(float x, float y, float z);
    Vec3 getPosition();

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
    void draw();

    Mat4 getModelMatrix();


    SceneNode* findNode(const std::string& name);

private:
    SceneGraph* getScene();
    Mat4 billboardMatrix(Mat4 View);
    Mat4 getTranslation();
    Quat getOrientation();
    Mat4 getScale();
};

class SceneGraph{
private:
    Camera* camera;
    SceneNode* root;
public:
    SceneGraph(Camera* cam);

    void destroy();

    Mat4 getViewMatrix();
    Mat4 getProjectionMatrix();

    Camera* getCamera();
    SceneNode* getRoot();

    SceneNode* findNode(const std::string& name);

    void update(int dt);
    void draw();
};

#endif //CGJM_SCENEGRAPH_H
