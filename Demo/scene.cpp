//
// Created by joao on 11/18/17.
//

#include "scene.h"
#include <string>
#include <sstream>
#include <math.h>
#include <iostream>
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


    //auto camera = ResourceManager::Factory::createFreeCamera(FREE_CAM, Vec3(20.0f, 5.0f, 5.0f), Quat());
    auto camera = ResourceManager::Factory::createSphereCamera(FREE_CAM, 20.0f, Vec3(20.0, 0.0, -20.0f), Quat());
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
        lamp->setCone(-0.3f, -1.0f, 0.0f, (float)M_PI/4.0f);
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
    H3DMesh* carModel = (H3DMesh*)rm->getMesh(CAR);
    auto carNode = new SceneNode(CAR, carModel, h3dShader);
    carModel->setMaterialUploadCallback(materialUploadCallback);
    carNode->translate(20.0f, 0.0f, -20.0f);
    root->addChild(carNode);

    /*Place the exhaust emmitters*/

    auto fireTexture = ResourceManager::Factory::createTexture(FIRE_PARTICLE);
    auto scene = ResourceManager::getInstance()->getScene(SCENE);
    auto pool = ResourceManager::Factory::createParticlePool(POOL, MAX_PARTICLES, scene);
    auto fireShader = ResourceManager::getInstance()->getShader(FIRE_SHADER);
    auto exhaustLeft = ResourceManager::Factory::createParticleEmmiter(LEFT_EXHAUST, pool, fireShader, fireTexture,
                                                                       Vec3(0.0f, 0.000001f, -0.000003f), Vec3(0.0f, 0.0001f, 0.003f),
                                                                       Vec3(-0.8f, 0.4f, 5.9f), 0.3, 0.0012f);
    auto exhaustRight = ResourceManager::Factory::createParticleEmmiter(RIGHT_EXHAUST, pool, fireShader, fireTexture,
                                                                       Vec3(0.0f, 0.00001f, 0.00003f), Vec3(0.0f, 0.0f, 0.0f),
                                                                       Vec3(0.8f, 0.4f, 5.9f), 0.3, 0.0012f);
    exhaustLeft->setParticleLifeDecayRate(0.006f);
    exhaustRight->setParticleLifeDecayRate(0.006f);
    //exhaustLeft->setRandomAcceleration(Vec3(1.0f, 0.0f, 1.0f));
    //exhaustRight->setRandomAcceleration(Vec3(0.0f, 0.0f, 0.0f));

    carNode->addChild(exhaustLeft);
    carNode->addChild(exhaustRight);

    /*Place the sun*/
    auto sun = ResourceManager::Factory::createLight(SUN);
    sun->setColor(0.9f, 0.9f, 0.9f);
    sun->setPoint(10.0f, 20.0f, 0.0f);
    sun->setAttenuation(1.0f, 0.0f, 0.0f, -1.0f);
    root->addChild(sun);
    LightsManager::getInstance()->setEnabled(sun, true);


    /*Heat haze*/
    auto helperFBO = ResourceManager::getInstance()->getFrameBuffer(HELPER_FBO);
    auto heatShader = ResourceManager::getInstance()->getShader(HEAT_SHADER);
    auto hazeEmitter = ResourceManager::Factory::createParticleEmmiter(HEAT_EMITTER, pool, heatShader, helperFBO->getTexture(),
                                                                       Vec3(0.0f, 1e-8f, 0.0f), Vec3(0.0f, 0.0f, 3e-5f),
                                                                       Vec3(0.0f, 0.7f, 6.1f), 0.001, 0.0f);
    hazeEmitter->setRandomAcceleration(Vec3(4e-8f, 3e-10f, 1e-18f));
    hazeEmitter->setParticleLifeDecayRate(5e-5f);
    hazeEmitter->setProcessingLevel(HEAT_HAZE_LEVEL);
    hazeEmitter->emmit();
    hazeEmitter->update(20000); //Hack to get things going faster
    carNode->addChild(hazeEmitter);



    /*Prototype smoke*/
    auto smokeTexture = ResourceManager::Factory::createTexture(SMOKE_PARTICLE);
    auto smokeShader = ResourceManager::getInstance()->getShader(SMOKE_SHADER);
    auto smokeEmitter = ResourceManager::Factory::createParticleEmmiter(SMOKE_EMITTER, pool, smokeShader, smokeTexture,
                                                                        Vec3(0.0f, 0.01f, 0.0f), Vec3(0.0f, 0.0f, 0.0f),
                                                                        Vec3(10.0f, 1.0f, -10.0f), 0.01, 0.0f);
    smokeEmitter->setVelocity(Vec3(0.0f, 0.01f, 0.0f));
    smokeEmitter->setAcceleration(Vec3(0.0f, 0.0f, 0.0f));
    smokeEmitter->setRate(0.1f);
    smokeEmitter->emmit();
    root->addChild(smokeEmitter);

    /*Setup Final result*/
    Mesh* quad = new QuadMesh();
    rm->addMesh("quad", quad);
    auto viewportCamera = ResourceManager::Factory::createHUDCamera(ORTHO_CAM, -1, 1, 1, -1, 0, 1);
    SceneNode* final = ResourceManager::Factory::createScene(FINAL, viewportCamera);
    final->setMesh(quad);
    Shader* twoD = rm->getShader(TWO_D_SHADER);
    final->setShader(twoD);
    final->translate(0.0f, 0.0f, -0.1f);
}

