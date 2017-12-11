//
// Created by joao on 11/21/17.
//

#include "CGJengine.h"
#include "shaders.h"
#include "Constants.h"


void loadShaders(){
    int MVPLocation;
    int ModelLocation;
    int ViewLocation;
    int ProjectionLocation;
    int NormalLocation;
    int lightPositionLocation;
    int lightsEnabledLocation;
    int lightColorLocation;
    int lightConeLocation;
    int lightAttenuationLocation;

    /*H3D Shader*/
    auto h3dShader = ResourceManager::Factory::createShader(H3D_SHADER, "res/shaders/vert.glsl", "res/shaders/frag.glsl");
    h3dShader->setAttribLocation("inPositon", VERTICES__ATTR);
    h3dShader->setAttribLocation("inNormal", NORMALS__ATTR);
    h3dShader->setAttribLocation("inTexCoord", TEXCOORDS__ATTR);
    h3dShader->setAttribLocation("inJointIndex", BONEINDICES__ATTR);
    h3dShader->setAttribLocation("inJointWeight", BONEWEIGHTS__ATTR);
    h3dShader->setFragOutputLocation("color", 0);
    h3dShader->link();

    MVPLocation = h3dShader->getUniformLocation("MVP");
    ModelLocation = h3dShader->getUniformLocation("Model");
    ViewLocation = h3dShader->getUniformLocation("View");
    NormalLocation = h3dShader->getUniformLocation("NormalMatrix");
    h3dShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P){
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(P*V*M));
        glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(M));
        glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, glm::value_ptr(V));
        glUniformMatrix4fv(NormalLocation, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(V*M))));
    });


    lightPositionLocation = h3dShader->getUniformLocation("lightPosition_worldspace[0]");
    lightsEnabledLocation = h3dShader->getUniformLocation("lightsEnabled[0]");
    lightColorLocation = h3dShader->getUniformLocation("lightColor[0]");
    lightConeLocation = h3dShader->getUniformLocation("lightCone[0]");
    lightAttenuationLocation = h3dShader->getUniformLocation("lightAttenuation[0]");
    LightsManager::getInstance()->registerShader(h3dShader, [=](Vec3 color, Vec3 position, Vec4 cone,
                                                                Vec4 attenuation, int enabled, int i){
        glUniform1iv(lightsEnabledLocation+i, 1, &enabled);
        if(enabled) {
            glUniform3fv(lightColorLocation + i, 1, glm::value_ptr(color));
            glUniform4fv(lightConeLocation + i, 1, glm::value_ptr(cone));
            glUniform4fv(lightAttenuationLocation + i, 1, glm::value_ptr(attenuation));
            glUniform3fv(lightPositionLocation + i, 1, glm::value_ptr(position));
        }
    });



    /*Sky Shader*/
    auto skyShader = ResourceManager::Factory::createShader(SKY_SHADER, "res/shaders/skyv.glsl", "res/shaders/skyf.glsl");
    skyShader->setAttribLocation("aPos", VERTICES__ATTR);
    skyShader->link();
    MVPLocation = skyShader->getUniformLocation("MVP");
    skyShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P) {
        Mat4 rotOnlyMV = V*M;
        rotOnlyMV[3][0] = 0.0f;
        rotOnlyMV[3][1] = 0.0f;
        rotOnlyMV[3][2] = 0.0f;
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(P * rotOnlyMV));
    });

    /*2D shader*/
    auto twoDShader = ResourceManager::Factory::createShader(FINAL_SHADER, "res/shaders/2Dv.glsl", "res/shaders/2Df.glsl");
    twoDShader->setAttribLocation("inPosition", VERTICES__ATTR);
    twoDShader->link();
    MVPLocation = twoDShader->getUniformLocation("MVP");
    twoDShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P) {
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(P * V * M));
    });

    /*Quad shader*/
    auto quadShader = ResourceManager::Factory::createShader(QUAD_SHADER, "res/shaders/quadv.glsl", "res/shaders/2Df.glsl");
    quadShader->setAttribLocation("inPosition", VERTICES__ATTR);
    quadShader->link();
    MVPLocation = quadShader->getUniformLocation("MVP");
    quadShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P) {
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(P * V * M));
    });

    /*Heat shader*/
    auto heatShader = ResourceManager::Factory::createShader(HEAT_SHADER, "res/shaders/hazev.glsl", "res/shaders/hazef3.glsl");
    heatShader->setAttribLocation("vertex", PARTICLE_VERT_ATTR);
    heatShader->setAttribLocation("state", PARTICLE_STATE_ATTR);
    heatShader->link();
    ViewLocation = heatShader->getUniformLocation("View");
    ProjectionLocation = heatShader->getUniformLocation("Projection");
    heatShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P) {
        glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, glm::value_ptr(P));
        glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, glm::value_ptr(V));
    });

    /*Heat shader*/
    auto heatReflectionShader = ResourceManager::Factory::createShader(HEAT_REFLECTION_SHADER, "res/shaders/hazevReflection.glsl", "res/shaders/hazefReflection.glsl");
    heatReflectionShader->setAttribLocation("vertex", PARTICLE_VERT_ATTR);
    heatReflectionShader->setAttribLocation("state", PARTICLE_STATE_ATTR);
    heatReflectionShader->link();
    ViewLocation = heatReflectionShader->getUniformLocation("View");
    ProjectionLocation = heatReflectionShader->getUniformLocation("Projection");
    heatReflectionShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P) {
        glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, glm::value_ptr(P));
        glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, glm::value_ptr(V));
    });

    /*Heat reflection shader*/
    auto heatRShader = ResourceManager::Factory::createShader(HEAT_SPOT_REFLECTION_SHADER, "res/shaders/heatReflectionV.glsl", "res/shaders/heatReflectionF.glsl");
    heatRShader->setAttribLocation("vertex", VERTICES__ATTR);
    heatRShader->link();
    ModelLocation = heatRShader->getUniformLocation("Model");
    ViewLocation = heatRShader->getUniformLocation("View");
    ProjectionLocation = heatRShader->getUniformLocation("Projection");
    GLint textureLoc = glGetUniformLocation(heatRShader->getShader(), "map");
    GLint mirrorLoc = glGetUniformLocation(heatRShader->getShader(), "mirror");
    heatRShader->use();
    glUniform1i(textureLoc, 0);
    glUniform1i(mirrorLoc, 1);
    heatRShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P) {
        glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(M));
        glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, glm::value_ptr(V));
        glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, glm::value_ptr(P));
    });

    /*Smoke shader*/
    auto smokeShader = ResourceManager::Factory::createShader(SMOKE_SHADER, "res/shaders/smokev.glsl", "res/shaders/particlef.glsl");
    smokeShader->setAttribLocation("vertex", PARTICLE_VERT_ATTR);
    smokeShader->setAttribLocation("state", PARTICLE_STATE_ATTR);
    smokeShader->link();
    ViewLocation = smokeShader->getUniformLocation("View");
    ProjectionLocation = smokeShader->getUniformLocation("Projection");
    smokeShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P){
        glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, glm::value_ptr(P));
        glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, glm::value_ptr(V));
    });

    /*Fire shader*/
    auto fireShader = ResourceManager::Factory::createShader(FIRE_SHADER, "res/shaders/firev.glsl", "res/shaders/firef.glsl");
    fireShader->setAttribLocation("vertex", PARTICLE_VERT_ATTR);
    fireShader->setAttribLocation("state", PARTICLE_STATE_ATTR);
    fireShader->link();
    ViewLocation = fireShader->getUniformLocation("View");
    ProjectionLocation = fireShader->getUniformLocation("Projection");
    fireShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P){
        glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, glm::value_ptr(P));
        glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, glm::value_ptr(V));
    });
}