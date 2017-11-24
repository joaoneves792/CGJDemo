//
// Created by joao on 11/18/17.
//

#include "scene.h"
#include <string>
#include <sstream>
#include <math.h>
#include "CGJengine.h"
#include "shaders.h"
#include "meshes.h"
#include "Constants.h"
#include "SceneNode.h"

void setupScene(){
    auto rm = ResourceManager::getInstance();
    loadMeshes();
    loadShaders();

    ResourceManager::Factory::createFrameBuffer(MAIN_FBO, WIN_X, WIN_Y);
    ResourceManager::Factory::createFrameBuffer(HELPER_FBO, WIN_X, WIN_Y);


    auto camera = ResourceManager::Factory::createFreeCamera(FREE_CAM, Vec3(0.0f, 5.0f, 5.0f), Quat(0.1, Vec3(0.0f, 1.0f, 0.0f)));
    camera->perspective((float)M_PI/4.0f, 0, 0.1f, 550.0f);
    SceneNode* root = ResourceManager::Factory::createScene(SCENE, camera);

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
    for(int i=0; i<ROAD_SEGMENTS; i++){
        std::stringstream name;
        name << ROAD << i;
        auto roadPart = new SceneNode(name.str(), roadModel, h3dShader);
        roadPart->translate(0.0f, 0.0f, 60.0f*(i-ROAD_SEGMENTS/2));
        road->addChild(roadPart);

        /*Create the lights*/
        std::stringstream lightName;
        lightName << LAMP_POST << i;
        auto lamp = ResourceManager::Factory::createLight(lightName.str());
        lamp->setColor(0.8f, 0.8f, 0.3f);
        lamp->setCone(-0.3f, -1.0f, 0.0f, M_PI/4.0f);
        lamp->setAttenuation(1.0f, 0.0f, 0.0f, 30.0f);
        lamp->setPosition(20.0f, 10.0f, -16.0f);
        LightsManager::getInstance()->setEnabled(lamp, false);

        roadPart->addChild(lamp);

    }

    /*Place the sky*/
    auto sky = new SceneNode(SKY, rm->getMesh(SKY), rm->getShader(SKY_SHADER));
    sky->scale(350.0f, 350.0f, 350.0f);
    root->addChild(sky);

    /*Place the car (should be last because of transparency on glasses)*/
    H3DMesh* chargerModel = (H3DMesh*)rm->getMesh(CAR);
    auto charger = new SceneNode(CAR, chargerModel, h3dShader);
    chargerModel->setMaterialUploadCallback(materialUploadCallback);
    charger->translate(20.0f, 0.0f, -20.0f);
    root->addChild(charger);


    /*Place the sun*/
    auto sun = ResourceManager::Factory::createLight(SUN);
    sun->setColor(0.9f, 0.9f, 0.9f);
    sun->setPoint(10.0f, 20.0f, 0.0f);
    sun->setAttenuation(1.0f, 0.0f, 0.0f, -1.0f);
    root->addChild(sun);
    LightsManager::getInstance()->setEnabled(sun, true);


    /*PROTOTYPE CODE*/
    auto particleRoot = new SceneNode("particleRoot");
    particleRoot->translate(0.0f, 1.0f, 7.0f);
    particleRoot->setBillboard(true);
    particleRoot->setProcessingLevel(HEAT_HAZE_LEVEL);
    charger->addChild(particleRoot);

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
        quadNode->setProcessingLevel(HEAT_HAZE_LEVEL);
        particleRoot->addChild(quadNode);
    }

    /*Setup Final result*/
    auto viewportCamera = ResourceManager::Factory::createHUDCamera(ORTHO_CAM, -1, 1, 1, -1, 0, 1);
    SceneNode* final = ResourceManager::Factory::createScene(FINAL, viewportCamera);
    final->setMesh(quad);
    Shader* twoD = rm->getShader(TWO_D_SHADER);
    final->setShader(twoD);
    final->translate(0.0f, 0.0f, -0.1f);
}

