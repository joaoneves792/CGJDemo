//
// Created by joao on 11/23/17.
//

#ifndef CGJDEMO_LIGHTSMANAGER_H
#define CGJDEMO_LIGHTSMANAGER_H


#include <map>
#include "LightNode.h"

class LightsManager {
private:
    static LightsManager* ourInstance;
    std::map<LightNode*, int> enabledLights;
    std::map<Shader*, std::function<void(float* color, float* position, float* cone,
                                         float* attenuation, int enabled, int i)>> shaderUpdateCallback;
    LightsManager() = default;
public:
    static LightsManager* getInstance();
    void addLight(LightNode* l);
    void setEnabled(LightNode* l, bool enabled);
    void registerShader(Shader* shader, std::function<void(float* color, float* position, float* cone,
                                                           float* attenuation, int enabled, int i)> callback);

    void uploadLights();
};


#endif //CGJDEMO_LIGHTSMANAGER_H
