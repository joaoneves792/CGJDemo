//
// Created by joao on 11/15/17.
//

#include <functional>
#include <unordered_map>
#include <map>
#include <GL/freeglut.h>
#include "InputManager.h"

InputManager* InputManager::ourInstance = nullptr;

InputManager* InputManager::getInstance() {
    if(ourInstance == nullptr){
        ourInstance = new InputManager();
    }
    return ourInstance;
}

void InputManager::setActionInterval(unsigned int milliseconds) {
    this->updateInterval = milliseconds;
    ++updateCallbackCounter;
    glutTimerFunc(milliseconds, InputManager::update, updateCallbackCounter);
}

void InputManager::update(int value) {
    auto im = InputManager::getInstance();

    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int timeDelta = (int)(currentTime-(im->lastUpdateTime));
    im->lastUpdateTime = currentTime;

    if(value != im->updateCallbackCounter)
        return;

    //Call the callbacks
    if(im->mouseMovementCallback != nullptr)
        im->mouseMovementCallback(im->mouseX, im->mouseY, timeDelta);

    for(auto it=im->pendingKeyCallbacks.begin(); it!=im->pendingKeyCallbacks.end(); it++){
        it->second(timeDelta);
    }

    for(auto it=im->pendingSpecialKeyCallbacks.begin(); it!=im->pendingSpecialKeyCallbacks.end(); it++){
        it->second(timeDelta);
    }

    glutTimerFunc(im->updateInterval, InputManager::update, im->updateCallbackCounter);
}


void InputManager::keyDown(unsigned char key) {
    if(keyCallbacks.find(key) != keyCallbacks.end())
        pendingKeyCallbacks[key] = keyCallbacks[key];
}

void InputManager::specialKeyDown(int key) {
    if(specialKeyCallbacks.find(key) != specialKeyCallbacks.end())
        pendingSpecialKeyCallbacks[key] = specialKeyCallbacks[key];
}

void InputManager::keyUp(unsigned char key) {
    auto it = pendingKeyCallbacks.find(key);
    if(it!=pendingKeyCallbacks.end())
        pendingKeyCallbacks.erase(it);
}

void InputManager::specialKeyUp(int key) {
    auto it = pendingSpecialKeyCallbacks.find(key);
    if(it!=pendingSpecialKeyCallbacks.end())
        pendingSpecialKeyCallbacks.erase(it);
}

void InputManager::mouseMovement(int x, int y) {
    mouseX = x;
    mouseY = y;
}

void InputManager::addKeyAction(unsigned char key, std::function<void(int dt)> callback) {
    keyCallbacks[key] = callback;
}

void InputManager::addSpecialKeyAction(int key, std::function<void(int dt)> callback) {
    specialKeyCallbacks[key] = callback;
}

void InputManager::setMouseAction(std::function<void(int x, int y, int dt)> callback) {
    mouseMovementCallback = callback;
}



