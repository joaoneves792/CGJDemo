//
// Created by joao on 11/18/17.
//

#include "scene.h"
#include <string>
#include "CGJengine.h"


void loadMeshes(){
    Mesh* table = new OBJMesh("res/table.obj");
    H3DMesh* ashouka = new H3DMesh("res/AT_base.h3d");
    ResourceManager::getInstance()->addMesh("table", table);
    ResourceManager::getInstance()->addMesh("ashouka", ashouka);
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

    auto h3dShader = new Shader("res/h3dv.glsl", "res/frag.glsl");
    h3dShader->setAttribLocation("inPositon", VERTICES__ATTR);
    h3dShader->setAttribLocation("inNormal", NORMALS__ATTR);
    h3dShader->setAttribLocation("inTexCoord", TEXCOORDS__ATTR);
    h3dShader->setAttribLocation("inJointIndex", BONEINDICES__ATTR);
    h3dShader->setAttribLocation("inJointWeight", BONEWEIGHTS__ATTR);
    h3dShader->link();
    h3dShader->setMVPFunction([=](Mat4 M, Mat4 V, Mat4 P){
        int MVPLocation = shader->getUniformLocation("MVP");
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, (P*V*M).transpose());
        int ModelLocation = shader->getUniformLocation("Model");
        glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, M.transpose());
        int ViewLocation = shader->getUniformLocation("View");
        glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, V.transpose());
    });
    ResourceManager::getInstance()->addShader("h3d", h3dShader);
}

void setupScene(const std::string& sceneName){
    auto rm = ResourceManager::getInstance();

    loadMeshes();
    loadShaders();

    auto camera = new FreeCamera(Vec3(0.0f, 5.0f, 5.0f), Quat(0.1, Vec3(0.0f, 1.0f, 0.0f)));

    auto root = new SceneNode("root");

    auto table = new SceneNode("table", rm->getMesh("table"), rm->getShader("table"));
    auto ashouka = new SceneNode("ashouka", rm->getMesh("ashouka"), rm->getShader("h3d"));
    H3DMesh* m = (H3DMesh*)rm->getMesh("ashouka");
    Shader* s = rm->getShader("h3d");
    m->setMaterialUploadCallback([=](float* ambient, float* diffuse, float* specular, float* emissive, float shininess, float transparency) {
        GLint ambientLoc = glGetUniformLocation(s->getShader(), "ambient");
        GLint diffuseLoc = glGetUniformLocation(s->getShader(), "diffuse");
        GLint specularLoc = glGetUniformLocation(s->getShader(), "specular");
        GLint emissiveLoc = glGetUniformLocation(s->getShader(), "emissive");
        GLint shininessLoc = glGetUniformLocation(s->getShader(), "shininess");
        GLint transparencyLoc = glGetUniformLocation(s->getShader(), "transparency");

        glUniform4fv(ambientLoc, 1, ambient);
        glUniform4fv(diffuseLoc, 1, diffuse);
        glUniform4fv(specularLoc, 1, specular);
        glUniform4fv(emissiveLoc, 1, emissive);

        glUniform1f(shininessLoc, shininess);
        glUniform1f(transparencyLoc, 1 - (transparency));
    });


    root->addChild(table);
    root->addChild(ashouka);


    auto scene = new SceneGraph(camera, root);
    rm->addScene(sceneName, scene);


}