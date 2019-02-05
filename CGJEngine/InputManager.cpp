//
// Created by joao on 11/15/17.
//

#include <functional>
#include <unordered_map>
#include <map>
#include <chrono>
#include <iostream>
#include "InputManager.h"



InputManager* InputManager::_ourInstance = nullptr;

InputManager* InputManager::getInstance() {
    if(_ourInstance == nullptr){
        _ourInstance = new InputManager();
        _ourInstance->_lastUpdateTime = getTime();
        _ourInstance->_updateInterval = 0;
    }
    return _ourInstance;
}

void InputManager::setActionInterval(unsigned int milliseconds) {
    this->_updateInterval = milliseconds;
}

void InputManager::update() {
    static auto im = InputManager::getInstance();

    long currentTime = getTime();
    int timeDelta = (int)(currentTime-(im->_lastUpdateTime));
    im->_lastUpdateTime = currentTime;

    if(timeDelta < (int)im->_updateInterval)
        return;

    //Call the callbacks
    if(im->_mouseMovementCallback != nullptr && im->_mouseDirty && im->_grabMouse) {
        im->_mouseMovementCallback(im->_mouseX, im->_mouseY, timeDelta);
        im->_mouseDirty = false;
    }

    for(auto it=im->_pendingKeyCallbacks.begin(); it!=im->_pendingKeyCallbacks.end(); it++){
        it->second(timeDelta);
    }

    for(auto it=im->_pendingSpecialKeyCallbacks.begin(); it!=im->_pendingSpecialKeyCallbacks.end(); it++){
        it->second(timeDelta);
    }
}


void InputManager::keyDown(unsigned char key) {
    if(_keyCallbacks.find(key) != _keyCallbacks.end())
        _pendingKeyCallbacks[key] = _keyCallbacks[key];

    //Call it imediately if its a one time thing
    if(_keyOnceCallbacks.find(key) != _keyOnceCallbacks.end())
        _keyOnceCallbacks[key]();
}

void InputManager::specialKeyDown(int key) {
    if(_specialKeyCallbacks.find(key) != _specialKeyCallbacks.end())
        _pendingSpecialKeyCallbacks[key] = _specialKeyCallbacks[key];

    if(_specialKeyOnceCallbacks.find(key) != _specialKeyOnceCallbacks.end())
        _specialKeyOnceCallbacks[key]();
}

void InputManager::keyUp(unsigned char key) {
    auto it = _pendingKeyCallbacks.find(key);
    if(it!=_pendingKeyCallbacks.end())
        _pendingKeyCallbacks.erase(it);
}

void InputManager::specialKeyUp(int key) {
    auto it = _pendingSpecialKeyCallbacks.find(key);
    if(it!=_pendingSpecialKeyCallbacks.end())
        _pendingSpecialKeyCallbacks.erase(it);
}

void InputManager::mouseMovement(int x, int y) {
    _mouseX = x;
    _mouseY = y;
    _mouseDirty = true;
}

void InputManager::addKeyAction(unsigned char key, std::function<void(int dt)> callback) {
    _keyCallbacks[key] = callback;
}

void InputManager::addKeyActionOnce(unsigned char key, std::function<void()> callback) {
    _keyOnceCallbacks[key] = callback;
}

void InputManager::addSpecialKeyAction(int key, std::function<void(int dt)> callback) {
    _specialKeyCallbacks[key] = callback;
}

void InputManager::addSpecialKeyActionOnce(int key, std::function<void()> callback) {
    _specialKeyOnceCallbacks[key] = callback;
}


void InputManager::setMouseAction(std::function<void(int x, int y, int dt)> callback) {
    _mouseMovementCallback = callback;
}

void InputManager::grabMouse(bool grab) {
    _grabMouse = grab;
}

long InputManager::getTime() {
    return std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch()).count();
}