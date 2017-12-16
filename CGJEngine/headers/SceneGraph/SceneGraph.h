//
// Created by joao on 11/12/17.
//

#ifndef CGJM_SCENEGRAPH_H
#define CGJM_SCENEGRAPH_H

#include <functional>
#include <list>
#include <string>
#include <unordered_map>

#include "glm_wrapper.h"
#include "Shader.h"
#include "Meshes/Mesh.h"
#include "Cameras/Camera.h"
#include "SceneNode.h"

#define ROOT "root"
#define DEFAULT_LEVEL 0
#define DEFAULT_PARTICLES_LEVEL 5


class SceneGraph{
private:
    Camera* _camera;
    SceneNode* _root;
    std::unordered_map<std::string, SceneNode*> _lookUpTable;
    friend class SceneNode;
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
    void draw(int level);
};

#endif //CGJM_SCENEGRAPH_H
