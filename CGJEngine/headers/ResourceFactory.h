//
// Created by joao on 11/21/17.
//

#ifndef CGJDEMO_RESOURCEFACTORY_H
#define CGJDEMO_RESOURCEFACTORY_H

#include "Mesh.h"
#include "Shader.h"
#include "SceneGraph.h"
#include "LightNode.h"

class ResourceFactory {
public:
    static Mesh* createMesh(const std::string& name, const std::string& filename);
    static Shader* createShader(const std::string& name, const std::string& vertexShader, const std::string fragmentShader);
    static SceneNode* createScene(const std::string& name, Camera* camera);
    static LightNode* createLight(const std::string& name);
};


#endif //CGJDEMO_RESOURCEFACTORY_H
