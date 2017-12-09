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

    ResourceManager::Factory::createMSAAFrameBuffer(MAIN_FBO, WIN_X, WIN_Y, MSAA);
    ResourceManager::Factory::createTextureFrameBuffer(HELPER_FBO, WIN_X, WIN_Y);

    //auto camera = ResourceManager::Factory::createFreeCamera(FREE_CAM, Vec3(20.0f, GROUND_LEVEL, 0.0f), Quat());
    auto camera = ResourceManager::Factory::createSphereCamera(FREE_CAM, 20.0f, Vec3(20.0f, GROUND_LEVEL, -20.0f), Quat());
    camera->perspective((float)PI/4.0f, 0, 0.1f, 550.0f);
    SceneNode* root = ResourceManager::Factory::createScene(SCENE, camera);
    root->translate(0.0f, GROUND_LEVEL, 0.0f);

    /*Setup material handling for H3D models*/
    Shader* h3dShader = rm->getShader(H3D_SHADER);
    GLint ambientLoc = glGetUniformLocation(h3dShader->getShader(), "ambient");
    GLint diffuseLoc = glGetUniformLocation(h3dShader->getShader(), "diffuse");
    GLint specularLoc = glGetUniformLocation(h3dShader->getShader(), "specular");
    GLint emissiveLoc = glGetUniformLocation(h3dShader->getShader(), "emissive");
    GLint shininessLoc = glGetUniformLocation(h3dShader->getShader(), "shininess");
    GLint transparencyLoc = glGetUniformLocation(h3dShader->getShader(), "transparency");
    GLint textureLoc = glGetUniformLocation(h3dShader->getShader(), "texture_sampler");
    GLint environmentLoc = glGetUniformLocation(h3dShader->getShader(), "environment");
    auto materialUploadCallback = [=](float* ambient, float* diffuse, float* specular, float* emissive, float shininess, float transparency) {
        glUniform1i(textureLoc, TEXTURE_SLOT);
        glUniform1i(environmentLoc, ENVIRONMENT_SLOT);

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
    int li = 0;
    for(int i=0; i<ROAD_SEGMENTS; i++){
        std::stringstream name;
        name << ROAD << i;
        auto roadPart = new SceneNode(name.str(), roadModel, h3dShader);
        roadPart->translate(0.0f, 0.0f, ROAD_LENGTH*(i-ROAD_SEGMENTS/2));
        road->addChild(roadPart);

        if(std::abs(i-ROAD_SEGMENTS/2) <= ACTIVE_LAMPS/2) {
            /*Create the lights*/
            std::stringstream lightName;
            lightName << LAMP_POST << li;
            ++li;
            auto lamp = ResourceManager::Factory::createLight(lightName.str());
            lamp->setColor(0.8f, 0.8f, 0.3f);
            lamp->setCone(-0.5f, -1.0f, 0.0f, (float) PI / 4.0f);
            lamp->setAttenuation(1.0f, 0.0f, 0.0f, 30.0f);
            lamp->setPosition(20.0f, 10.0f, -16.0f);
            LightsManager::getInstance()->setEnabled(lamp, false);
            roadPart->addChild(lamp);
        }
    }

    /*Place the sky*/
    auto skyCubeMap = ResourceManager::Factory::createCubeMap(SKY_CUBE_MAP,
                                                              "res/skybox/posx.jpg", "res/skybox/negx.jpg",
                                                              "res/skybox/posy.jpg", "res/skybox/negy.jpg",
                                                              "res/skybox/posz.jpg", "res/skybox/negz.jpg");
    auto skyShader = rm->getShader(SKY_SHADER);
    skyShader->use();
    glUniform1f(skyShader->getUniformLocation("brightness"), 1.0f);
    auto sky = new SceneNode(SKY, rm->getMesh(SKY), skyShader);
    sky->setPreDraw([=](){
       skyCubeMap->bindCubeMap();
    });
    root->addChild(sky);


    /*Place the sun*/
    auto sun = ResourceManager::Factory::createLight(SUN);
    sun->setColor(0.9f, 0.9f, 0.9f);
    sun->setPoint(10.0f, 20.0f, 0.0f);
    sun->setAttenuation(1.0f, 0.0f, 0.0f, -1.0f);
    root->addChild(sun);
    LightsManager::getInstance()->setEnabled(sun, true);

    /*Create environment map*/
    auto environment = ResourceManager::Factory::createCubeMap(ENVIRONMENT,
                                                               "res/environment/right.png", "res/environment/left.png",
                                                               "res/environment/top.png", "res/environment/bottom.png",
                                                               "res/environment/back.png", "res/environment/front.png");
    auto bindEnvironment = [=](){
        glActiveTexture(GL_TEXTURE0+ENVIRONMENT_SLOT);
        environment->bindCubeMap();
        glActiveTexture(GL_TEXTURE0);
    };

    /*Place the car (should be last because of transparency on glasses)*/
    H3DMesh* carModel = (H3DMesh*)rm->getMesh(CAR);
    auto carNode = new SceneNode(CAR, carModel, h3dShader);
    carModel->setMaterialUploadCallback(materialUploadCallback);
    carNode->translate(20.0f, 0.0f, -20.0f);
    carNode->setPreDraw(bindEnvironment);
    root->addChild(carNode);

    /*Place the wheels*/
    auto wheelMesh = (H3DMesh*)rm->getMesh(WHEEL);
    wheelMesh->setMaterialUploadCallback(materialUploadCallback);

    auto rearAxis = new SceneNode(REAR_AXIS);
    rearAxis->translate(0.0f, 0.67f, 3.77f);
    carNode->addChild(rearAxis);
    auto rrWheel = new SceneNode(RRWHEEL,wheelMesh, h3dShader);
    rrWheel->setPreDraw(bindEnvironment);
    rrWheel->translate(2.05f, 0.0f, 0.0f);
    rrWheel->rotate(0.0f, 1.0f, 0.0f, PI);
    rearAxis->addChild(rrWheel);
    auto rlWheel = new SceneNode(RLWHEEL,wheelMesh, h3dShader);
    rlWheel->setPreDraw(bindEnvironment);
    rlWheel->translate(-2.05f, 0.0f, 0.0f);
    rearAxis->addChild(rlWheel);

    auto frontAxis = new SceneNode(FRONT_AXIS);
    frontAxis->translate(0.0f, 0.67f, -2.71f);
    carNode->addChild(frontAxis);
    auto frWheel = new SceneNode(FRWHEEL,wheelMesh, h3dShader);
    frWheel->setPreDraw(bindEnvironment);
    frWheel->translate(2.17f, 0.0f, 0.0f);
    frWheel->rotate(0.0f, 1.0f, 0.0f, PI);
    frontAxis->addChild(frWheel);
    auto flWheel = new SceneNode(FLWHEEL, wheelMesh, h3dShader);
    flWheel->setPreDraw(bindEnvironment);
    flWheel->translate(-2.17f, 0.0f, 0.0f);
    frontAxis->addChild(flWheel);

    /*Place the exhaust emmitters*/
    auto fireTexture = ResourceManager::Factory::createTexture(FIRE_PARTICLE);
    auto scene = ResourceManager::getInstance()->getScene(SCENE);
    auto pool = ResourceManager::Factory::createParticlePool(POOL, MAX_PARTICLES, scene);
    auto fireShader = ResourceManager::getInstance()->getShader(FIRE_SHADER);
    auto exhaustLeft = ResourceManager::Factory::createParticleEmmiter(LEFT_EXHAUST, pool, fireShader, fireTexture,
                                                                       Vec3(0.0f, 0.000001f, -0.000003f), Vec3(0.0f, 0.0001f, 0.003f),
                                                                       Vec3(-0.8f, 0.4f, 5.8f), 0.3, 0.0012f);
    auto exhaustRight = ResourceManager::Factory::createParticleEmmiter(RIGHT_EXHAUST, pool, fireShader, fireTexture,
                                                                       Vec3(0.0f, 0.00001f, 0.00003f), Vec3(0.0f, 0.0f, 0.0f),
                                                                       Vec3(0.8f, 0.4f, 5.8f), 0.3, 0.0012f);
    exhaustLeft->setParticleLifeDecayRate(0.006f);
    exhaustRight->setParticleLifeDecayRate(0.006f);
    exhaustLeft->setRandomAcceleration(Vec3(3e-10f, 3e-8f, 0.0f));
    exhaustRight->setRandomAcceleration(Vec3(3e-10f, 3e-8f, 0.0f));

    carNode->addChild(exhaustLeft);
    carNode->addChild(exhaustRight);

    /*Heat haze*/
    auto helperFBO = ResourceManager::getInstance()->getFrameBuffer(HELPER_FBO);
    auto heatShader = ResourceManager::getInstance()->getShader(HEAT_SHADER);
    auto hazeEmitter = ResourceManager::Factory::createParticleEmmiter(HEAT_EMITTER, pool, heatShader, ((TextureFrameBuffer*)helperFBO)->getTexture(),
                                                                       Vec3(0.0f, 1e-8f, 0.0f), Vec3(0.0f, 0.0f, 3e-5f),
                                                                       Vec3(0.0f, 0.7f, 6.1f), 0.002, 0.0f);
    hazeEmitter->setRandomAcceleration(Vec3(4e-8f, 3e-10f, 1e-12f));
    hazeEmitter->setParticleLifeDecayRate(5e-5f);
    hazeEmitter->setProcessingLevel(HEAT_HAZE_LEVEL);
    hazeEmitter->emmit();
    carNode->addChild(hazeEmitter);

    for(int i=0;i<1000;i++)
        hazeEmitter->update(20); //Hack to get things going faster


    /*Setup HUD*/
    Mesh* quad = new QuadMesh();
    rm->addMesh("quad", quad);
    auto viewportCamera = ResourceManager::Factory::createHUDCamera(ORTHO_CAM, -1, 1, 1, -1, 0, 1);
    SceneNode* final = ResourceManager::Factory::createScene(HUD, viewportCamera);

    SceneNode* credits = new SceneNode(CREDITS, quad, ResourceManager::getInstance()->getShader(QUAD_SHADER));
    auto creditsTexture = ResourceManager::Factory::createTexture("res/credits.png");
    credits->setPreDraw([=](){
       creditsTexture->bind();
    });
    credits->scale(0.15f, 0.15f, 1.0f);
    credits->translate(0.8f, -0.9f, 0.0f);
    final->addChild(credits);
}

