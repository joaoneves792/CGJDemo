//
// Created by joao on 11/15/17.
//

#ifndef CGJM_RESOURCEMANAGER_H
#define CGJM_RESOURCEMANAGER_H


#include <unordered_map>
#include <string>
#include "SceneGraph.h"
#include "Shader.h"
#include "Mesh.h"

class ResourceManager {
private:
    static ResourceManager* ourInstance;
    std::unordered_map<std::string, Shader*> shaders;
    std::unordered_map<std::string, Mesh*> meshes;
    std::unordered_map<std::string, SceneGraph*> scenes;
    std::unordered_map<std::string, Camera*> cameras;
    ResourceManager() = default;

    void __destroyShader(Shader* shader);
    void __destroyMesh(Mesh* mesh);
    void __destroyScene(SceneGraph* scene);
    void __destroyCamera(Camera* camera);
public:
    static ResourceManager* getInstance();
    static void deleteInstance();
    void addShader(std::string name, Shader* shader);
    void addMesh(std::string name, Mesh* mesh);
    void addScene(std::string name, SceneGraph* scene);
    void addCamera(std::string name, Camera* camera);

    Shader* getShader(std::string name);
    Mesh* getMesh(std::string name);
    SceneGraph* getScene(std::string name);
    Camera* getCamera(std::string name);

    void destroyShader(std::string name);
    void destroyMesh(std::string name);
    void destroyScene(std::string name);
    void destroyCamera(std::string name);

    void destroyAllShaders();
    void destroyAllMeshes();
    void destroyAllScenes();
    void destroyAllCameras();

    void destroyEverything();
};


#endif //CGJM_RESOURCEMANAGER_H
