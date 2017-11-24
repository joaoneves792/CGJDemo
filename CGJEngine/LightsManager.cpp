//
// Created by joao on 11/23/17.
//

#include "LightsManager.h"

LightsManager* LightsManager::ourInstance = nullptr;

LightsManager* LightsManager::getInstance() {
    if(ourInstance == nullptr){
        ourInstance = new LightsManager();
    }
    return ourInstance;
}

void LightsManager::addLight(LightNode *l) {
    enabledLights[l] = 1;
}

void LightsManager::setEnabled(LightNode *l, bool enabled) {
    enabledLights[l] = (enabled)? 1:0;
}

void LightsManager::registerShader(Shader *shader,
                                   std::function<void(float *color, float* position, float *cone,
                                                      float *attenuation, int enabled, int i)> callback) {
    shaderUpdateCallback[shader] = callback;
}

void LightsManager::uploadLights() {
    int i = 0;
    for(auto it=enabledLights.begin(); it!=enabledLights.end(); it++){
        for(auto sit=shaderUpdateCallback.begin(); sit!=shaderUpdateCallback.end(); sit++){
            sit->first->use();
            sit->second((float*)it->first->getColor(), (float*)it->first->getPosition(),
                        (float*)it->first->getCone(), (float*)it->first->getAttenuation(), it->second, i);
        }
        i++;
    }
}