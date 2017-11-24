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
#include "SceneNode.h"
#include "LightNode.h"
#include "Camera.h"
#include "FreeCamera.h"
#include "SphereCamera.h"
#include "HUDCamera.h"
#include "FrameBuffer.h"

class ResourceManager {
private:
    static ResourceManager* ourInstance;
    std::unordered_map<std::string, Shader*> shaders;
    std::unordered_map<std::string, Mesh*> meshes;
    std::unordered_map<std::string, SceneGraph*> scenes;
    std::unordered_map<std::string, Camera*> cameras;
    std::unordered_map<std::string, FrameBuffer*> fbos;
public:
    class Factory{
    public:
        static Mesh* createMesh(const std::string& name, const std::string& filename);
        static Shader* createShader(const std::string& name, const std::string& vertexShader, const std::string fragmentShader);
        static SceneNode* createScene(const std::string& name, Camera* camera);
        static LightNode* createLight(const std::string& name);
        static FrameBuffer* createFrameBuffer(const std::string& name, int x, int y);
        static FreeCamera* createFreeCamera(const std::string& name, Vec3 position, Quat orientation);
        static SphereCamera* createSphereCamera(const std::string& name, float distance, Vec3 center, Quat orientation);
        static HUDCamera* createHUDCamera(const std::string& name,
                                          float left, float right, float top, float bottom, float near, float far);
    };

private:
    ResourceManager() = default;

    void __destroyShader(Shader* shader);
    void __destroyMesh(Mesh* mesh);
    void __destroyScene(SceneGraph* scene);
    void __destroyCamera(Camera* camera);
    void __destroyFrameBuffer(FrameBuffer* fbo);
public:
    static ResourceManager* getInstance();
    static void deleteInstance();
    void addShader(std::string name, Shader* shader);
    void addMesh(std::string name, Mesh* mesh);
    void addScene(std::string name, SceneGraph* scene);
    void addCamera(std::string name, Camera* camera);
    void addFrameBuffer(std::string name, FrameBuffer* fbo);

    Shader* getShader(std::string name);
    Mesh* getMesh(std::string name);
    SceneGraph* getScene(std::string name);
    Camera* getCamera(std::string name);
    FrameBuffer* getFrameBuffer(std::string name);

    void destroyShader(std::string name);
    void destroyMesh(std::string name);
    void destroyScene(std::string name);
    void destroyCamera(std::string name);
    void destroyFrameBuffer(std::string name);

    void destroyAllShaders();
    void destroyAllMeshes();
    void destroyAllScenes();
    void destroyAllCameras();
    void destroyAllFrameBuffers();

    void destroyEverything();
};


#endif //CGJM_RESOURCEMANAGER_H
