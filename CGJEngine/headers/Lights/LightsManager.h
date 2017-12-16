//
// Created by joao on 11/23/17.
//

#ifndef CGJDEMO_LIGHTSMANAGER_H
#define CGJDEMO_LIGHTSMANAGER_H


#include <map>
#include "SceneGraph/LightNode.h"

class LightsManager {
private:
    static LightsManager* _ourInstance;
    std::map<LightNode*, int> _enabledLights;
    std::map<Shader*, std::function<void(Vec3 color, Vec3 position, Vec4 cone,
                                         Vec4 attenuation, int enabled, int i)>> _shaderUpdateCallback;
    LightsManager() = default;
public:
    static LightsManager* getInstance();
    void addLight(LightNode* l);
    void setEnabled(LightNode* l, bool enabled);
    void registerShader(Shader* shader, std::function<void(Vec3 color, Vec3 position, Vec4 cone,
                                                           Vec4 attenuation, int enabled, int i)> callback);

    void uploadLights();
};


#endif //CGJDEMO_LIGHTSMANAGER_H
