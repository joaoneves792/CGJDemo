//
// Created by joao on 11/18/17.
//

#include "scene.h"
#include <string>
#include <sstream>
#include "CGJengine.h"
#include "shaders.h"
#include "meshes.h"
#include "ResourceNames.h"

void setupScene(){
    auto rm = ResourceManager::getInstance();
    loadMeshes();
    loadShaders();

    auto camera = new FreeCamera(Vec3(0.0f, 5.0f, 5.0f), Quat(0.1, Vec3(0.0f, 1.0f, 0.0f)));
    SceneNode* root = ResourceFactory::createScene(SCENE, camera);

    /*Setup material handling for H3D models*/
    Shader* h3dShader = rm->getShader(H3D_SHADER);
    auto materialUploadCallback = [=](float* ambient, float* diffuse, float* specular, float* emissive, float shininess, float transparency) {
        GLint ambientLoc = glGetUniformLocation(h3dShader->getShader(), "ambient");
        GLint diffuseLoc = glGetUniformLocation(h3dShader->getShader(), "diffuse");
        GLint specularLoc = glGetUniformLocation(h3dShader->getShader(), "specular");
        GLint emissiveLoc = glGetUniformLocation(h3dShader->getShader(), "emissive");
        GLint shininessLoc = glGetUniformLocation(h3dShader->getShader(), "shininess");
        GLint transparencyLoc = glGetUniformLocation(h3dShader->getShader(), "transparency");

        glUniform4fv(ambientLoc, 1, ambient);
        glUniform4fv(diffuseLoc, 1, diffuse);
        glUniform4fv(specularLoc, 1, specular);
        glUniform4fv(emissiveLoc, 1, emissive);

        glUniform1f(shininessLoc, shininess);
        glUniform1f(transparencyLoc, 1 - (transparency));
    };


    /*Place the road*/
    H3DMesh* roadModel = (H3DMesh*)rm->getMesh(ROAD);
    roadModel->setMaterialUploadCallback(materialUploadCallback);
    auto road = new SceneNode(ROAD);
    root->addChild(road);
    for(int i=-6; i<6; i++){
        std::stringstream name;
        name << ROAD << i;
        auto roadPart = new SceneNode(name.str(), roadModel, h3dShader);
        roadPart->translate(0.0f, 0.0f, 60.0f*i);
        road->addChild(roadPart);
    }

    /*Place the sky*/
    auto sky = new SceneNode(SKY, rm->getMesh(SKY), rm->getShader(SKY_SHADER));
    sky->scale(350.0f, 350.0f, 350.0f);
    root->addChild(sky);

    /*Place the car (should be last because of transparency on glasses)*/
    H3DMesh* chargerModel = (H3DMesh*)rm->getMesh(CAR);
    auto charger = new SceneNode(CAR, chargerModel, h3dShader);
    chargerModel->setMaterialUploadCallback(materialUploadCallback);
    charger->translate(10.0f, 0.0f, -20.0f);
    root->addChild(charger);



    /*PROTOTYPE CODE*/
    SceneNode* particleRoot = ResourceFactory::createScene(POST, camera);
    particleRoot->translate(10.0f, 1.0f, -13.0f);
    particleRoot->setBillboard(true);
    //particleRoot->scale(2.0f, 2.0f, 2.0f);

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

