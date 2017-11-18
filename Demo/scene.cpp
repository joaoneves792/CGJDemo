//
// Created by joao on 11/18/17.
//

#include "scene.h"
#include <string>
#include "CGJengine.h"


void loadMeshes(){
    OBJMesh* table = new OBJMesh("res/table.obj");
    ResourceManager::getInstance()->addMesh("table", table);
}

void loadShaders(){
    auto shader = new Shader("res/tablev.glsl", "res/tablef.glsl");
    shader->setAttribLocation("inPosition", VERTICES__ATTR);
    shader->setAttribLocation("inNormal", NORMALS__ATTR);
    shader->link();
    shader->setMVPFunction([=](Mat4 M, Mat4 V, Mat4 P){
        int MVPLocation = shader->getUniformLocation("MVP");
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, (P*V*M).transpose());
        int ModelLocation = shader->getUniformLocation("Model");
        glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, M.transpose());
        int ViewLocation = shader->getUniformLocation("View");
        glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, V.transpose());
    });
    ResourceManager::getInstance()->addShader("table", shader);
}

void setupScene(const std::string& sceneName){
    auto rm = ResourceManager::getInstance();

    loadMeshes();
    loadShaders();

    auto camera = new FreeCamera(Vec3(0.0f, 5.0f, 5.0f), Quat(0.1, Vec3(0.0f, 1.0f, 0.0f)));
    auto table = new SceneNode("table", rm->getMesh("table"), rm->getShader("table"));

    auto scene = new SceneGraph(camera, table);
    rm->addScene(sceneName, scene);


}