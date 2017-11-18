//
// Created by joao on 11/15/17.
//

#ifndef CGJM_RESOURCEMANAGER_H
#define CGJM_RESOURCEMANAGER_H


#include <unordered_map>
#include <string>
#include "SceneGraph.h"
#include "Shader.h"
#include "OBJMesh.h"

class ResourceManager {
private:
    static ResourceManager* ourInstance;
    std::unordered_map<std::string, Shader*> shaders;
    std::unordered_map<std::string, OBJMesh*> meshes;
    std::unordered_map<std::string, SceneGraph*> scenes;
    ResourceManager() = default;

    void __destroyShader(Shader* shader);
    void __destroyMesh(OBJMesh* mesh);
    void __destroyScene(SceneGraph* scene);
public:
    static ResourceManager* getInstance();
    static void deleteInstance();
    void addShader(std::string name, Shader* shader);
    void addMesh(std::string name, OBJMesh* mesh);
    void addScene(std::string name, SceneGraph* scene);

    Shader* getShader(std::string name);
    OBJMesh* getMesh(std::string name);
    SceneGraph* getScene(std::string name);

    void destroyShader(std::string name);
    void destroyMesh(std::string name);
    void destroyScene(std::string name);

    void destroyAllShaders();
    void destroyAllMeshes();
    void destroyAllScenes();

    void destroyEverything();
};


#endif //CGJM_RESOURCEMANAGER_H
