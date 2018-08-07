//
// Created by joao on 11/18/17.
//

#include "scene.h"
#include <string>
#include <sstream>
#include <ctgmath>
#include <cmath>
#include <iostream>
#include "CGJengine.h"
#include "shaders.h"
#include "meshes.h"
#include "constants.h"
#include "SceneGraph/SceneNode.h"

void setupScene(){
    auto rm = ResourceManager::getInstance();
    loadMeshes();
    loadShaders();

    /*Generic quad mesh for multiple purposes*/
    Mesh* quad = new QuadMesh();
    rm->addMesh(QUAD, quad);


    auto camera = ResourceManager::Factory::createFreeCamera(SPHERE_CAM, Vec3(20.0f, GROUND_LEVEL, 0.0f), Quat(1.0f, 0.0f, 0.0f, 0.0f));
    //auto camera = ResourceManager::Factory::createSphereCamera(SPHERE_CAM, 20.0f, Vec3(20.0f, GROUND_LEVEL, -20.0f), Quat(1.0f, 0.0f, 0.0f, 0.0f));
    /*auto camera = ResourceManager::Factory::createHUDCamera(SPHERE_CAM, -20.0f, 20.0f, 10.0f, -20.0f, 0.1f, 100.0f, true);
    camera->changeOrientation(-PI/2.0f, 0.0f, 0.0f);
    camera->changeOrientation(0.0f, PI/4.0f, 0.0f);
    camera->setPosition(50.0f, 40.0f, -20.0f);*/
    camera->perspective((float)PI/4.0f, 0, 1.0f, 1000.0f);
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
    GLint shadowShaderProgram = (GLint)rm->getShader(SHADOW_SHADER)->getShader();
    auto materialUploadCallback = [=](float ambient, float* diffuse, float* specular, float* emissive, float shininess, float transparency) {
        GLint shader = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &shader);
        if(shader != shadowShaderProgram) {
            glUniform1fv(ambientLoc, 1, &ambient);
            glUniform4fv(diffuseLoc, 1, diffuse);
            glUniform4fv(specularLoc, 1, specular);
            glUniform4fv(emissiveLoc, 1, emissive);

            glUniform1f(shininessLoc, shininess);
            glUniform1f(transparencyLoc, 1 - (transparency));
        }
    };

    /*Create environment map*/
    auto environment = ResourceManager::Factory::createCubeMap(ENVIRONMENT,
                                                               "res/environment/right.jpg", "res/environment/left.jpg",
                                                               "res/environment/top.jpg", "res/environment/bottom.jpg",
                                                               "res/environment/back.jpg", "res/environment/front.jpg");
    auto bindEnvironment = [=](){
        glActiveTexture(GL_TEXTURE0+ENVIRONMENT_SLOT);
        environment->bindCubeMap();
        glActiveTexture(GL_TEXTURE0);
    };

    /*Place the sky*/
    auto skyCubeMap = ResourceManager::Factory::createCubeMap(SKY_CUBE_MAP,
                                                              "res/skybox/posx.jpg", "res/skybox/negx.jpg",
                                                              "res/skybox/posy.jpg", "res/skybox/negy.jpg",
                                                              "res/skybox/posz.jpg", "res/skybox/negz.jpg");
    auto skyShader = rm->getShader(SKY_SHADER);
    skyShader->use();
    glUniform1f(skyShader->getUniformLocation("brightness"), 1.0f);
    auto sky = new SceneNode(SKY, rm->getMesh(SKY), skyShader);
    auto bindSkyEnvironment = [=](){
        glActiveTexture(GL_TEXTURE0+ENVIRONMENT_SLOT);
        skyCubeMap->bindCubeMap();
        glActiveTexture(GL_TEXTURE0);
    };
    sky->setPreDraw([=](){
        glActiveTexture(GL_TEXTURE0);
        skyCubeMap->bindCubeMap();
    });
    root->addChild(sky);

    /*Place the road*/

    H3DMesh* roadModel = (H3DMesh*)rm->getMesh(ROAD);
    H3DMesh* asphaltModel = (H3DMesh*)rm->getMesh(ASPHALT);
    H3DMesh* grassModel = (H3DMesh*)rm->getMesh(GRASS);
    Shader* asphaltShader = rm->getShader(ASPHALT_SHADER);
    Shader* parallaxShader = rm->getShader(PARALLAX_SHADER);
    Shader* grassShader = rm->getShader(GRASS_SHADER);
    const int GRASS_LAYERS = 40;
    const float GRASS_LENGHT = 0.4;
    Noise* grass = new Noise(GRASS_LAYERS-1, 512);
    grass->setColor(0.0f, 0.7f, 0.0f, 0.0f, 0.7f, 0.0f);
    //grass->setColor(0.0f, 0.7f, 0.0f, 0.0f, 0.1f, 0.0f);
    grass->generateSimpleNoise(10000, 12345);
    //grass->generatePerlinNoise(2, 2.0f, 0.5f); // <- good for generating thick bushes!
    rm->addNoise("grassNoise", grass);
    int furLengthLocation = grassShader->getUniformLocation("furLength");
    int uvScaleLocation = grassShader->getUniformLocation("uvScale");
    int timeLoc = grassShader->getUniformLocation("time");
    auto grassLayerCallback = [=](int layer){
        GLint shader = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &shader);
        if(shader != shadowShaderProgram) {
            glUniform1f(furLengthLocation, GRASS_LENGHT * ((float)layer/(float)GRASS_LAYERS));
            //glUniform1f(uvScaleLocation, 1.0f + 1.0f * ((float)layer/(float)GRASS_LAYERS));
            glUniform1f(uvScaleLocation, 1.2f);
            glActiveTexture(GL_TEXTURE1);
            grass->bindLayerTexture(layer-1);
            //grassNoiseTexture->bind();
        }
    };

    auto road = new SceneNode(ROAD);
    root->addChild(road);
    int li = 0;
    for(int i=0; i<ROAD_SEGMENTS; i++){
        std::stringstream name, sceneryName, asphaltName, grassName;
        name << ROAD << i;
        sceneryName << ROAD << "scenery" << i;
        asphaltName << ASPHALT << i;
        grassName << GRASS << i;
        auto roadPart = new SceneNode(name.str());
        auto roadScenery = new SceneNode(sceneryName.str(), roadModel, parallaxShader);
        auto asphalt = new SceneNode(asphaltName.str(), asphaltModel, asphaltShader);
        auto grass = new SceneNode(grassName.str(), grassModel, grassShader);
        grass->setLayerCount(GRASS_LAYERS);
        grass->setLayerCallback(grassLayerCallback);
        grass->setUpdateCallback([=](int dt){
            static float time = 0.0f;
            time = time + dt/10000.0f;
            //time = time - (int)time;
            grassShader->use();
            glUniform1f(timeLoc,time);

        });
        asphalt->setPreDraw(bindSkyEnvironment);
        roadPart->translate(0.0f, 0.0f, ROAD_LENGTH*(i-ROAD_SEGMENTS/2));
        roadScenery->setPreDraw([=](){
            glDisable(GL_CULL_FACE);
        });
        roadScenery->setPostDraw([=](){
            glEnable(GL_CULL_FACE);
        });
        road->addChild(roadPart);
        roadPart->addChild(roadScenery);
        roadPart->addChild(asphalt);
        roadPart->addChild(grass);

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

    /*Place the sun*/
    auto sun = ResourceManager::Factory::createLight(SUN);
    sun->setColor(0.9f, 0.9f, 0.9f);
    Vec3 sunPos = Vec3(40.0f, 80.0f, 0.0f);
    sun->setPoint(sunPos[0], sunPos[1], sunPos[2]);
    sun->setAttenuation(1.0f, 0.0f, 0.0f, -1.0f);
    root->addChild(sun);
    LightsManager::getInstance()->setEnabled(sun, true);


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

    exhaustLeft->setProcessingLevel(SMOOTH_PARTICLES_LEVEL);
    exhaustRight->setProcessingLevel(SMOOTH_PARTICLES_LEVEL);

    carNode->addChild(exhaustLeft);
    carNode->addChild(exhaustRight);

    /*Smoke*/
    auto smokeTexture = ResourceManager::Factory::createTexture(SMOKE_PARTICLE);
    auto smokeShader = rm->getShader(SMOKE_SHADER);
    const Vec3 smoke_accelaration = Vec3(0.0f, 9e-8f, 1e-5f);
    const Vec3 smoke_random_accelaration = Vec3(2e-6f, 3e-10f, 1e-7f);
    const Vec3 smoke_velocity = Vec3(0.0f, 9e-4f, 6e-5f);
    const float smoke_rate = 0.03f;
    const float smoke_decay = 0.00001f;
    const float smoke_particle_decay = 5e-4f;
    auto smokeLeft = ResourceManager::Factory::createParticleEmmiter(SMOKE_EMITTER_RL, pool, smokeShader, smokeTexture,
                                                                        smoke_accelaration, smoke_velocity,
                                                                        Vec3(0.0f), smoke_rate, smoke_decay);
    smokeLeft->setRandomAcceleration(smoke_random_accelaration);
    smokeLeft->setParticleLifeDecayRate(smoke_particle_decay);
    smokeLeft->setProcessingLevel(SMOOTH_PARTICLES_LEVEL);
    rlWheel->addChild(smokeLeft);

    auto smokeRight = ResourceManager::Factory::createParticleEmmiter(SMOKE_EMITTER_RR, pool, smokeShader, smokeTexture,
                                                                     smoke_accelaration, smoke_velocity,
                                                                     Vec3(0.0f), smoke_rate, smoke_decay);
    smokeRight->setRandomAcceleration(smoke_random_accelaration);
    smokeRight->setParticleLifeDecayRate(smoke_particle_decay);
    smokeRight->setProcessingLevel(SMOOTH_PARTICLES_LEVEL);
    rrWheel->addChild(smokeRight);

    /*Heat haze*/

    auto noise  = ResourceManager::Factory::createTexture("res/noise1.png");
    auto heatShader = ResourceManager::getInstance()->getShader(HEAT_SHADER);
    auto hazeEmitter = ResourceManager::Factory::createParticleEmmiter(HEAT_EMITTER, pool, heatShader, nullptr,
                                                                       Vec3(0.0f, 1e-8f, 0.0f), Vec3(0.0f, 3e-5f, 3e-5f),
                                                                       Vec3(0.0f, 0.3f, 6.4f), 0.002, 0.0f);
    hazeEmitter->setRandomAcceleration(Vec3(2e-8f, 3e-10f, 1e-8f));
    hazeEmitter->setParticleLifeDecayRate(5e-5f);
    hazeEmitter->setProcessingLevel(HEAT_HAZE_LEVEL);
    hazeEmitter->emmit();
    hazeEmitter->setPreDraw([=](){
        glActiveTexture(GL_TEXTURE1);
        noise->bind();
        glActiveTexture(GL_TEXTURE0);
    });

        carNode->addChild(hazeEmitter);
    for(int i=0;i<1000;i++)
        hazeEmitter->update(20); //Hack to get things going faster

    //Quads for distance heat distortion
    auto distanceHeatShader = rm->getShader(HEAT_DISTANCE_SHADER);
    timeLoc = distanceHeatShader->getUniformLocation("time");
    auto distanceHeat = new SceneNode(DISTANCE_HEAT);
    distanceHeat->scale(15.0f, 5.0f, 0.0f);
    distanceHeat->translate(20.0f, 5.0f, 0.0);
    distanceHeat->setBillboard(true);
    distanceHeat->setUpdateCallback([=](int dt){
        static float time = 0.0f;
        time = time + dt/30000.0f;
        time = time - (int)time;
        distanceHeatShader->use();
        glUniform1f(timeLoc,time);
    });

    root->addChild(distanceHeat);

    auto frontHeat = new SceneNode(FRONT_HEAT, quad, distanceHeatShader);
    frontHeat->setProcessingLevel(HEAT_HAZE_LEVEL);
    frontHeat->translate(0.0f, 0.0f, -HEAT_HAZE_DISTANCE);
    frontHeat->setPreDraw([=](){
        glActiveTexture(GL_TEXTURE1);
        noise->bind();
    });

    distanceHeat->addChild(frontHeat);
    auto rearHeat = new SceneNode(REAR_HEAT, quad, distanceHeatShader);
    rearHeat->setProcessingLevel(HEAT_HAZE_LEVEL);
    rearHeat->translate(0.0f, 0.0f, HEAT_HAZE_DISTANCE);
    rearHeat->setPreDraw([=](){
        glActiveTexture(GL_TEXTURE1);
        noise->bind();
    });
    distanceHeat->addChild(rearHeat);



    /*Setup HUD*/
    auto creditsCamera = ResourceManager::Factory::createHUDCamera(BOTTOM_RIGHT_CAM, WIN_X, 0, WIN_Y, 0, 0, 1, false);
    SceneNode* credits = ResourceManager::Factory::createScene(CREDITS_HUD, creditsCamera);
    credits->setMesh(quad);
    credits->setShader(rm->getShader(QUAD_SHADER));
    auto creditsTexture = ResourceManager::Factory::createTexture("res/credits2.png");
    credits->setPreDraw([=](){
        glDisable(GL_CULL_FACE);
        creditsTexture->bind();
    });
    credits->setPostDraw([=](){
        glEnable(GL_CULL_FACE);
    });
    credits->scale(-100.0f, -50.0f, 1.0f);
    credits->translate(100.0f, 50.0f, -0.1f);


    /*Shadows*/
    auto shadowCamera = ResourceManager::Factory::createHUDCamera(SHADOW_CAMERA, -30.0f, 30.0f, 10.0f, -20.0f, 0.1f, 100.0f, true);
    shadowCamera->changeOrientation(-PI/2.0f, 0.0f, 0.0f);
    shadowCamera->changeOrientation(0.0f, PI/4.0f, 0.0f);
    shadowCamera->setPosition(43.5f, 40.0f, -20.0f);
    auto shadowShader = rm->getShader(SHADOW_SHADER);
    GLint MVPLoc = shadowShader->getUniformLocation("MVP");
    shadowShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P){
        Mat4 MVP = shadowCamera->getProjectionMatrix() *
                   shadowCamera->getViewMatrix() *
                   M;
        glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));
    });


}

void flames(int value){
    /*Timer callback function to emmit exhaust flames*/
    ((ParticleEmitterNode*)ResourceManager::getInstance()->getScene(SCENE)->findNode(RIGHT_EXHAUST))->emmit();
    ((ParticleEmitterNode*)ResourceManager::getInstance()->getScene(SCENE)->findNode(LEFT_EXHAUST))->emmit();
    auto nextInterval = (unsigned int)(((float)std::rand()/(float)RAND_MAX)*5000);
    glutTimerFunc(nextInterval, flames, 0);
}
