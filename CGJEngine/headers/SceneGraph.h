//
// Created by joao on 11/12/17.
//

#ifndef CGJM_SCENEGRAPH_H
#define CGJM_SCENEGRAPH_H

#include <functional>
#include <list>
#include <string>
#include <unordered_map>

#include "vec.h"
#include "mat.h"
#include "quat.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "SceneNode.h"

#define ROOT "root"

class SceneGraph{
private:
    Camera* camera;
    SceneNode* root;
    std::unordered_map<std::string, SceneNode*> lookUpTable;
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
