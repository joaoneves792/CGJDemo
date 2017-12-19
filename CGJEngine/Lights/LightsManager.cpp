//
// Created by joao on 11/23/17.
//

#include "Lights/LightsManager.h"

LightsManager* LightsManager::_ourInstance = nullptr;

LightsManager* LightsManager::getInstance() {
    if(_ourInstance == nullptr){
        _ourInstance = new LightsManager();
    }
    return _ourInstance;
}

void LightsManager::addLight(LightNode *l) {
    _enabledLights[l] = 1;
}

void LightsManager::setEnabled(LightNode *l, bool enabled) {
    _enabledLights[l] = (enabled)? 1:0;
}

void LightsManager::registerShader(Shader *shader,
                                   std::function<void(Vec3 color, Vec3 position, Vec4 cone,
                                                      Vec4 attenuation, int enabled, int i)> callback) {
    _shaderUpdateCallback[shader] = callback;
}

void LightsManager::uploadLights() {
    int i = 0;
    //Upload first enabled lights (this allows for optimizing the shaders)
    for(auto it=_enabledLights.begin(); it!=_enabledLights.end(); it++){
        if(it->second) {
            for (auto sit = _shaderUpdateCallback.begin(); sit != _shaderUpdateCallback.end(); sit++) {
                sit->first->use();
                sit->second(it->first->getColor(), it->first->getPositionWorldspace(),
                            it->first->getCone(), it->first->getAttenuation(), it->second, i);
            }
            i++;
        }
    }
    //Next upload the disabled lights (at least mark them as disabled)
    for(auto it=_enabledLights.begin(); it!=_enabledLights.end(); it++){
        if(!it->second) {
            for (auto sit = _shaderUpdateCallback.begin(); sit != _shaderUpdateCallback.end(); sit++) {
                sit->first->use();
                sit->second(it->first->getColor(), it->first->getPositionWorldspace(),
                            it->first->getCone(), it->first->getAttenuation(), it->second, i);
            }
            i++;
        }
    }
}