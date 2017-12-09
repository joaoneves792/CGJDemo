//
// Created by joao on 11/21/17.
//

#include <sstream>
#include "CGJengine.h"
#include "actions.h"
#include "Constants.h"
#include "Movement.h"
#include "SceneNode.h"

#define ESCAPE 27

void setupActions() {
    SceneGraph *scene = ResourceManager::getInstance()->getScene(SCENE);
    InputManager *im = InputManager::getInstance();

    im->setActionInterval(10); //Update every 10ms

    /*Update camera movement*/
    const float movementRate = 0.005f; //magic number
    const float cameraRate = (float)-1.0f*(3.1415f / (WinX * 2));

    im->setMouseAction([=](int x, int y, int timeDelta){
        int deltaX = (WinX / 2) - x;
        int deltaY = (WinY / 2) - y;
        glutWarpPointer(WinX / 2, WinY / 2);
        scene->getCamera()->changeOrientation(deltaX*cameraRate, deltaY*cameraRate, 0.0f);
    });

    im->addKeyAction('w', [=](int timeDelta){
        scene->getCamera()->move(0.0f, 0.0f, -(timeDelta * movementRate));
    });
    im->addKeyAction('a', [=](int timeDelta){
        scene->getCamera()->move(-timeDelta * movementRate, 0.0f, 0.0f);
    });
    im->addKeyAction('s', [=](int timeDelta){
        scene->getCamera()->move(0.0f, 0.0f, timeDelta * movementRate);
    });
    im->addKeyAction('d', [=](int timeDelta){
        scene->getCamera()->move(timeDelta * movementRate, 0.0f, 0.0f);
    });
    im->addKeyAction('q', [=](int timeDelta){
        scene->getCamera()->changeOrientation(0.0f, 0.0f, (timeDelta * movementRate));
    });
    im->addKeyAction('e', [=](int timeDelta){
        scene->getCamera()->changeOrientation(0.0f, 0.0f, -(timeDelta * movementRate));
    });
    im->addKeyAction(ESCAPE, [=](int timeDelta){
        glutLeaveMainLoop();
    });
    im->addKeyActionOnce('f', [=](){
        static bool accelerating = false;
        accelerating = !accelerating;
        if(accelerating){
            scene->findNode(ROAD)->setUpdateCallback(Movement::accelerate);
        }else {
            scene->findNode(ROAD)->setUpdateCallback(Movement::deccelerate);
        }
    });
    im->addKeyActionOnce('n', [=](){
        static bool night = false;
        night = !night;
        auto lm = LightsManager::getInstance();
        auto skyShader = ResourceManager::getInstance()->getShader(SKY_SHADER);
        GLint brightnessLoc = skyShader->getUniformLocation("brightness");
        if(night){
            for(int i=0; i<ROAD_SEGMENTS; i++) {
                std::stringstream lightName;
                lightName << LAMP_POST << i;
                LightNode* l = (LightNode*)scene->findNode(lightName.str());
                lm->setEnabled(l, true);
            }
            LightNode* l = (LightNode*)scene->findNode(SUN);
            lm->setEnabled(l, false);
            skyShader->use();
            glUniform1f(brightnessLoc, 0.5f);
        }else {
            for(int i=0; i<ROAD_SEGMENTS; i++) {
                std::stringstream lightName;
                lightName << LAMP_POST << i;
                LightNode* l = (LightNode*)scene->findNode(lightName.str());
                lm->setEnabled(l, false);
            }
            LightNode* l = (LightNode*)scene->findNode(SUN);
            lm->setEnabled(l, true);
            skyShader->use();
            glUniform1f(brightnessLoc, 1.0f);
        }
    });
    im->addKeyActionOnce('b', [=](){
        static bool blackHazeParticles = false;
        blackHazeParticles = !blackHazeParticles;
        auto shader = ResourceManager::getInstance()->getShader(HEAT_SHADER);
        auto uniformLocation = shader->getUniformLocation("black");
        shader->use();
        glUniform1f(uniformLocation, (float)blackHazeParticles);
        glUseProgram(0);
    });

}