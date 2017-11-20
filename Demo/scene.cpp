//
// Created by joao on 11/18/17.
//

#include "scene.h"
#include <string>
#include <sstream>
#include "CGJengine.h"


void loadMeshes(){
    //Mesh* table = new OBJMesh("res/table.obj");
    H3DMesh* charger = new H3DMesh("res/charger/charger.h3d");
    H3DMesh* road = new H3DMesh("res/Road/road.h3d");
    H3DMesh* sky = new H3DMesh("res/Road/sky.h3d");
    //ResourceManager::getInstance()->addMesh("table", table);
    ResourceManager::getInstance()->addMesh("charger", charger);
    ResourceManager::getInstance()->addMesh("road", road);
    ResourceManager::getInstance()->addMesh("sky", sky);
}

void loadShaders(){
    auto shader = new Shader("res/tablev.glsl", "res/tablef.glsl");
    shader->setAttribLocation("inPosition", VERTICES__ATTR);
    shader->setAttribLocation("inNormal", NORMALS__ATTR);
    shader->setFragOutputLocation("color", 0);
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
    h3dShader->setFragOutputLocation("color", 0);
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

    auto quadShader = new Shader("res/quadv.glsl", "res/quadf.glsl");
    quadShader->setAttribLocation("inPosition", VERTICES__ATTR);
    quadShader->link();
    quadShader->setMVPFunction([=](Mat4 M, Mat4 V, Mat4 P) {
        int MVPLocation = quadShader->getUniformLocation("MVP");
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, (P * V * M).transpose());
    });
    ResourceManager::getInstance()->addShader("quad", quadShader);

    auto skyShader = new Shader("res/skyv.glsl", "res/skyf.glsl");
    skyShader->setAttribLocation("inPosition", VERTICES__ATTR);
    skyShader->setAttribLocation("inTexCoord", TEXCOORDS__ATTR);
    skyShader->link();
    skyShader->setMVPFunction([=](Mat4 M, Mat4 V, Mat4 P) {
        int MVPLocation = quadShader->getUniformLocation("MVP");
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, (P * V * M).transpose());
    });
    ResourceManager::getInstance()->addShader("sky", skyShader);
}

void setupScene(const std::string& sceneName){
    auto rm = ResourceManager::getInstance();

    loadMeshes();
    loadShaders();

    auto camera = new FreeCamera(Vec3(0.0f, 5.0f, 5.0f), Quat(0.1, Vec3(0.0f, 1.0f, 0.0f)));

    auto scene = new SceneGraph(camera);
    rm->addScene(sceneName, scene);

    SceneNode* root = scene->getRoot();

    //auto table = new SceneNode("table", rm->getMesh("table"), rm->getShader("table"));
    Shader* s = rm->getShader("h3d");
    auto materialUploadCallback = [=](float* ambient, float* diffuse, float* specular, float* emissive, float shininess, float transparency) {
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
    };
    auto charger = new SceneNode("charger", rm->getMesh("charger"), rm->getShader("h3d"));
    H3DMesh* chargerModel = (H3DMesh*)rm->getMesh("charger");
    chargerModel->setMaterialUploadCallback(materialUploadCallback);
    charger->translate(10.0f, 0.0f, -20.0f);

    auto road = new SceneNode("road", rm->getMesh("road"), rm->getShader("h3d"));
    H3DMesh* roadModel = (H3DMesh*)rm->getMesh("road");
    roadModel->setMaterialUploadCallback(materialUploadCallback);

    auto sky = new SceneNode("sky", rm->getMesh("sky"), rm->getShader("sky"));
    sky->scale(100.0f, 100.0f, 100.0f);


    root->addChild(sky);
    root->addChild(road);
    root->addChild(charger);



    SceneGraph* pseudoParticles = new SceneGraph(camera);
    rm->addScene("particles", pseudoParticles);

    SceneNode* particleRoot = new SceneNode("particleRoot");
    particleRoot->translate(10.0f, 1.0f, -13.0f);
    particleRoot->setBillboard(true);
    //particleRoot->scale(2.0f, 2.0f, 2.0f);
    pseudoParticles->getRoot()->addChild(particleRoot);

    Mesh* quad = new QuadMesh();
    rm->addMesh("quad", quad);

    float uniqueness = 0.0f;
    for(int i=0; i<5; i++){
        std::stringstream name;
        name << "quad" << i;
        SceneNode* quadNode = new SceneNode(name.str(), quad, rm->getShader("quad"));
        uniqueness += i/5.0f;
        quadNode->setPreDraw([=](){
            Shader* shader = rm->getShader("quad");
            glUniform1f(shader->getUniformLocation("offset"), (float)(glutGet(GLUT_ELAPSED_TIME)/ 1000.0 * 2*3.14159 * uniqueness));
        });
        quadNode->translate(i/5.0f, i/5.0f, i/5.0f);
        particleRoot->addChild(quadNode);
    }

    //Camera* hudCamera = new FreeCamera(Vec3(0.0f, 0.0f, 0.1f), Quat(0.01f, Vec3(0.0f, 1.0f, 0.0f)));
   // hudCamera->ortho(-2, 2, 2, -2, 1, -1);

}