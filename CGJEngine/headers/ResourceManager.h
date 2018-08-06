//
// Created by joao on 11/15/17.
//

#ifndef CGJM_RESOURCEMANAGER_H
#define CGJM_RESOURCEMANAGER_H


#include <unordered_map>
#include <string>
#include <FBOs/GFrameBuffer.h>
#include <Textures/Fur.h>
#include "SceneGraph/SceneGraph.h"
#include "Shader.h"
#include "Meshes/Mesh.h"
#include "SceneGraph/SceneNode.h"
#include "SceneGraph/LightNode.h"
#include "Cameras/Camera.h"
#include "Cameras/FreeCamera.h"
#include "Cameras/SphereCamera.h"
#include "Cameras/HUDCamera.h"
#include "FBOs/FrameBuffer.h"
#include "FBOs/ColorTextureFrameBuffer.h"
#include "FBOs/MSFrameBuffer.h"
#include "FBOs/DoubleColorMSFrameBuffer.h"
#include "FBOs/DepthTextureFrameBuffer.h"
#include "Particles/ParticlePool.h"
#include "Textures/Texture.h"

class ResourceManager {
private:
    static ResourceManager* _ourInstance;
    std::unordered_map<std::string, Shader*> _shaders;
    std::unordered_map<std::string, Mesh*> _meshes;
    std::unordered_map<std::string, SceneGraph*> _scenes;
    std::unordered_map<std::string, Camera*> _cameras;
    std::unordered_map<std::string, FrameBuffer*> _fbos;
    std::unordered_map<std::string, ParticlePool*> _pools;
    std::unordered_map<std::string, Texture*> _textures;
    std::unordered_map<std::string, Fur*> _furs;
public:
    class Factory{
    public:
        static Mesh* createMesh(const std::string& name, const std::string& filename);
        static Shader* createShader(const std::string& name, const std::string& vertexShader,
                                    const std::string& fragmentShader);
        static SceneNode* createScene(const std::string& name, Camera* camera);
        static LightNode* createLight(const std::string& name);
        static FrameBuffer* createFrameBuffer(const std::string& name, int x, int y);
        static ColorTextureFrameBuffer* createColorTextureFrameBuffer(const std::string &name, int x, int y);
        static DepthTextureFrameBuffer* createDepthTextureFrameBuffer(const std::string& name, int x, int y);
        static GFrameBuffer* createGFrameBuffer(const std::string& name, int x, int y);
        static MSFrameBuffer* createMSAAFrameBuffer(const std::string& name, int x, int y, int msaaLevel);
        static DoubleColorMSFrameBuffer* createDoubleColorMSAAFrameBuffer(const std::string& name, int x, int y, int msaaLevel);
        static FreeCamera* createFreeCamera(const std::string& name, Vec3 position, Quat orientation);
        static SphereCamera* createSphereCamera(const std::string& name, float distance, Vec3 center, Quat orientation);
        static HUDCamera* createHUDCamera(const std::string& name,
                                          float left, float right, float top, float bottom, float near, float far, bool scale);
        static ParticlePool* createParticlePool(const std::string& name, int particleCount, SceneGraph* scene);
        static ParticleEmitterNode* createParticleEmmiter(const std::string& name, ParticlePool* pool,
                                                          Shader* shader, Texture* texture, Vec3 acceleration,
                                                          Vec3 velocity, Vec3 position, float rate, float rateDecay);
        static Texture* createTexture(const std::string& fileName);
        static Texture* createCubeMap(const std::string& name, const std::string &right, const std::string &left,
                                      const std::string &top, const std::string &bottom,
                                      const std::string &back, const std::string &front);
    };

private:
    ResourceManager() = default;

    void __destroyShader(Shader* shader);
    void __destroyMesh(Mesh* mesh);
    void __destroyScene(SceneGraph* scene);
    void __destroyCamera(Camera* camera);
    void __destroyFrameBuffer(FrameBuffer* fbo);
    void __destroyParticlePool(ParticlePool* pool);
    void __destroyTexture(Texture* texture);
    void __destroyFur(Fur* fur);
public:
    static ResourceManager* getInstance();
    static void deleteInstance();
    void addShader(const std::string& name, Shader* shader);
    void addMesh(const std::string& name, Mesh* mesh);
    void addScene(const std::string& name, SceneGraph* scene);
    void addCamera(const std::string& name, Camera* camera);
    void addFrameBuffer(const std::string& name, FrameBuffer* fbo);
    void addParticlePool(const std::string& name, ParticlePool* pool);
    void addTexture(const std::string& name, Texture* texture);
    void addFur(const std::string& name, Fur* fur);

    Shader* getShader(const std::string& name);
    Mesh* getMesh(const std::string& name);
    SceneGraph* getScene(const std::string& name);
    Camera* getCamera(const std::string& name);
    FrameBuffer* getFrameBuffer(const std::string& name);
    ParticlePool* getParticlePool(const std::string& name);
    Texture* getTexture(const std::string& name);
    Fur* getFur(const std::string& name);

    void destroyShader(const std::string& name);
    void destroyMesh(const std::string& name);
    void destroyScene(const std::string& name);
    void destroyCamera(const std::string& name);
    void destroyFrameBuffer(const std::string& name);
    void destroyParticlePool(const std::string& name);
    void destroyTexture(const std::string& name);
    void destroyFur(const std::string& name);

    void destroyAllShaders();
    void destroyAllMeshes();
    void destroyAllScenes();
    void destroyAllCameras();
    void destroyAllFrameBuffers();
    void destroyAllParticlePools();
    void destroyAllTextures();
    void destroyAllFurs();

    void destroyEverything();
};


#endif //CGJM_RESOURCEMANAGER_H
