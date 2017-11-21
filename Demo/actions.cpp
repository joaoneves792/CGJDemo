//
// Created by joao on 11/21/17.
//

#include "CGJengine.h"
#include "actions.h"
#include "ResourceNames.h"
#include "Road.h"

#define ESCAPE 27

void setupActions() {
    SceneGraph *scene = ResourceManager::getInstance()->getScene(SCENE);
    InputManager *im = InputManager::getInstance();

    im->setActionInterval(10); //Update every 10ms

    /*Update camera movement*/
    const float movementRate = 0.005; //magic number
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
        scene->getCamera()->changeOrientation(0.0f, 0.0f, -(timeDelta * movementRate));
    });
    im->addKeyAction('e', [=](int timeDelta){
        scene->getCamera()->changeOrientation(0.0f, 0.0f, (timeDelta * movementRate));
    });
    im->addKeyAction(ESCAPE, [=](int timeDelta){
        glutLeaveMainLoop();
    });
    im->addKeyActionOnce('f', [=](){
        static bool accelerating = false;
        accelerating = !accelerating;
        if(accelerating){
            scene->findNode(ROAD)->setUpdateCallback(Road::accelerate);
        }else {
            scene->findNode(ROAD)->setUpdateCallback(Road::deccelerate);
        }
    });

}