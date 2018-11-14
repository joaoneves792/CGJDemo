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
#include "Meshes/Mesh.h"
#include "Cameras/Camera.h"

class SceneGraph;

class SceneNode{
protected:
    SceneGraph* _scene;

    std::string _name;
    Mesh* _mesh;
    Shader* _shader;
    std::string _groupOnly;

    bool _billboard;
    Vec3 _position;
    Quat _orientation;
    Vec3 _size;

    bool _visible;
    int _layers;

    SceneNode* _parent;
    std::list<SceneNode*> _childs = {};

    std::function<void()> _preDraw;
    std::function<void()> _postDraw;
    std::function<void(int dt)> _updateCallback;
    std::function<void(int layer)> _layerCallback;

    int _processingLevel = 0;

public:
    SceneNode(std::string name);
    SceneNode(std::string name, Mesh* mesh);
    SceneNode(std::string name, Mesh* mesh, Shader* shader);

    virtual ~SceneNode() = default;

    std::string getName();

    void setMesh(Mesh* mesh);
    Mesh* getMesh();
    void setShader(Shader* shader);
    Shader* getShader();

    void setScene(SceneGraph* sceneGraph);

    virtual void setProcessingLevel(int level);

    void setPosition(float x, float y, float z);
    void translate(float x, float y, float z);
    virtual Vec3 getPositionWorldspace();

    void setOrientation(float x, float y, float z, float angle);
    void setOrientation(Quat quat);
    Quat getOrientation();
    void rotate(float x, float y, float z, float angle);
    void scale(float x, float y, float z);

    void setPreDraw(std::function<void()> callback);
    void setPostDraw(std::function<void()> callback);
    void setUpdateCallback(std::function<void(int dt)> callback);
    void setLayerCallback(std::function<void(int layer)> callback);

    void setGroupOnly(std::string name);

    void addChild(SceneNode* child);
    void destroy();

    void hidden(bool b);
    void setBillboard(bool billboarded);


    virtual void update(int dt);
    virtual void draw(int level,const Mat4& parentTranslate,
                      const Quat& parentOrientation, const Mat4& parentScale, Shader* shader);

    Mat4 getModelMatrix();

    void setLayerCount(int layers);


    SceneNode* findNode(const std::string& name);

protected:
    SceneGraph* getScene();
    Mat4 getTranslation();
    Mat4 getScale();
};
#endif //CGJDEMO_SCENENODE_H
