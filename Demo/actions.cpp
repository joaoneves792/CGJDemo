//
// Created by joao on 11/21/17.
//

#include <sstream>
#include <iostream>
#include "CGJengine.h"
#include "actions.h"
#include "constants.h"
#include "movement.h"
#include "SceneGraph/SceneNode.h"

#define ESCAPE 27
#define SPACE 0x20

void setupActions() {
    SceneGraph *scene = ResourceManager::getInstance()->getScene(SCENE);
    InputManager *im = InputManager::getInstance();
    static ParticleEmitterNode* smokeLR = (ParticleEmitterNode*)
            ResourceManager::getInstance()->getScene(SCENE)->findNode(SMOKE_EMITTER_RL);
    static ParticleEmitterNode* smokeRR = (ParticleEmitterNode*)
            ResourceManager::getInstance()->getScene(SCENE)->findNode(SMOKE_EMITTER_RR);

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

    im->addKeyActionOnce('g', [=](){
        static bool grabMouse = true;
        grabMouse = !grabMouse;
        im->grabMouse(grabMouse);
        if(grabMouse)
            glutSetCursor(GLUT_CURSOR_NONE);
        else
            glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
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
        scene->getCamera()->changeOrientation(0.0f, 0.0f, -(timeDelta * movementRate/4));
    });
    im->addKeyAction('e', [=](int timeDelta){
        scene->getCamera()->changeOrientation(0.0f, 0.0f, (timeDelta * movementRate/4));
    });
    im->addKeyAction(ESCAPE, [=](int timeDelta){
        glutLeaveMainLoop();
    });
    im->addKeyActionOnce('f', [=](){
        static bool accelerating = false;
        accelerating = !accelerating;
        if(accelerating){
            scene->findNode(ROAD)->setUpdateCallback(Movement::accelerate);
            smokeLR->emmit();
            smokeRR->emmit();
        }else {
            scene->findNode(ROAD)->setUpdateCallback(Movement::deccelerate);
        }
    });
    im->addSpecialKeyActionOnce(GLUT_KEY_F11, [=](){
       glutFullScreenToggle();
    });
    im->addKeyActionOnce(' ', [=](){
        VRCamera* cam = (VRCamera*)ResourceManager::getInstance()->getCamera(SPHERE_CAM);
        cam->recenter();
    });
    im->addKeyActionOnce('r',[=](){
        int w = 256;
        int h = 256;
        ResourceManager::getInstance()->getFrameBuffer(MAIN_FBO)->resize(w, h);
        //ResourceManager::getInstance()->getFrameBuffer(SIDE_FBO1)->resize(w, h);
        //ResourceManager::getInstance()->getFrameBuffer(SIDE_FBO2)->resize(w, h);
        ResourceManager::getInstance()->getFrameBuffer(SIDE_FBO3)->resize(w, h);
        //ResourceManager::getInstance()->getFrameBuffer(SHADOW_FBO)->resize(w, h);
    });

    im->addKeyActionOnce('n', [=](){
        static bool night = false;
        night = !night;
        auto lm = LightsManager::getInstance();
        auto skyShader = ResourceManager::getInstance()->getShader(SKY_SHADER);
        auto asphaltShader = ResourceManager::getInstance()->getShader(ASPHALT_SHADER);
        GLint brightnessLoc = skyShader->getUniformLocation("brightness");
        GLint asphaltNightLoc = asphaltShader->getUniformLocation("night");
        if(night){
            for(int i=0; i<ACTIVE_LAMPS; i++) {
                std::stringstream lightName;
                lightName << LAMP_POST << i;
                LightNode* l = (LightNode*)scene->findNode(lightName.str());
                lm->setEnabled(l, true);
            }
            LightNode* l = (LightNode*)scene->findNode(SUN);
            lm->setEnabled(l, false);
            skyShader->use();
            glUniform1f(brightnessLoc, 0.5f);
            asphaltShader->use();
            glUniform1f(asphaltNightLoc, 1.0f);
            scene->findNode(DISTANCE_HEAT)->hidden(true);
        }else {
            for(int i=0; i<ACTIVE_LAMPS; i++) {
                std::stringstream lightName;
                lightName << LAMP_POST << i;
                LightNode* l = (LightNode*)scene->findNode(lightName.str());
                lm->setEnabled(l, false);
            }
            LightNode* l = (LightNode*)scene->findNode(SUN);
            lm->setEnabled(l, true);
            skyShader->use();
            glUniform1f(brightnessLoc, 1.0f);
            asphaltShader->use();
            glUniform1f(asphaltNightLoc, 0.0f);
            scene->findNode(DISTANCE_HEAT)->hidden(false);
        }
    });
}