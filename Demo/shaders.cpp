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
    auto h3dShader = ResourceManager::Factory::createShader(H3D_SHADER, "res/shaders/h3dv.glsl", "res/shaders/h3df.glsl");
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
    GLint textureLoc = glGetUniformLocation(h3dShader->getShader(), "texture_sampler");
    GLint environmentLoc = glGetUniformLocation(h3dShader->getShader(), "environment");
    h3dShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P){
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(P*V*M));
        glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(M));
        glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, glm::value_ptr(V));
        glUniformMatrix4fv(NormalLocation, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(V*M))));
    });
    h3dShader->use();
    glUniform1i(textureLoc, TEXTURE_SLOT);
    glUniform1i(environmentLoc, ENVIRONMENT_SLOT);

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

    /*Asphalt Shader*/
    auto asphaltShader = ResourceManager::Factory::createShader(ASPHALT_SHADER, "res/shaders/h3dv.glsl", "res/shaders/asphaltf.glsl");
    asphaltShader->setAttribLocation("inPositon", VERTICES__ATTR);
    asphaltShader->setAttribLocation("inNormal", NORMALS__ATTR);
    asphaltShader->setAttribLocation("inTexCoord", TEXCOORDS__ATTR);
    asphaltShader->setFragOutputLocation("color", 0);
    asphaltShader->link();

    MVPLocation = asphaltShader->getUniformLocation("MVP");
    ModelLocation = asphaltShader->getUniformLocation("Model");
    ViewLocation = asphaltShader->getUniformLocation("View");
    NormalLocation = asphaltShader->getUniformLocation("NormalMatrix");
    asphaltShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P){
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(P*V*M));
        glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(M));
        glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, glm::value_ptr(V));
        glUniformMatrix4fv(NormalLocation, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(V*M))));
    });
    textureLoc = asphaltShader->getUniformLocation("texture_sampler");
    environmentLoc = asphaltShader->getUniformLocation("environment");
    asphaltShader->use();
    glUniform1i(textureLoc, TEXTURE_SLOT);
    glUniform1i(environmentLoc, ENVIRONMENT_SLOT);

    lightPositionLocation = asphaltShader->getUniformLocation("lightPosition_worldspace[0]");
    lightsEnabledLocation = asphaltShader->getUniformLocation("lightsEnabled[0]");
    lightColorLocation = asphaltShader->getUniformLocation("lightColor[0]");
    lightConeLocation = asphaltShader->getUniformLocation("lightCone[0]");
    lightAttenuationLocation = asphaltShader->getUniformLocation("lightAttenuation[0]");
    LightsManager::getInstance()->registerShader(asphaltShader, [=](Vec3 color, Vec3 position, Vec4 cone,
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

    /*Quad shader*/
    auto quadShader = ResourceManager::Factory::createShader(QUAD_SHADER, "res/shaders/quadv.glsl", "res/shaders/2Df.glsl");
    quadShader->setAttribLocation("inPosition", VERTICES__ATTR);
    quadShader->link();
    MVPLocation = quadShader->getUniformLocation("MVP");
    quadShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P) {
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(P * V * M));
    });

    /*Heat shader*/
    auto heatShader = ResourceManager::Factory::createShader(HEAT_SHADER, "res/shaders/hazev.glsl", "res/shaders/hazef6.glsl");
    heatShader->setAttribLocation("vertex", PARTICLE_VERT_ATTR);
    heatShader->setAttribLocation("state", PARTICLE_STATE_ATTR);
    heatShader->link();
    GLint renderderTextureLoc = heatShader->getUniformLocation("renderedTexture");
    GLint noiseLoc = heatShader->getUniformLocation("noiseTexture");
    heatShader->use();
    glUniform1i(renderderTextureLoc, 0);
    glUniform1i(noiseLoc, 1);
    ViewLocation = heatShader->getUniformLocation("View");
    ProjectionLocation = heatShader->getUniformLocation("Projection");
    heatShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P) {
        glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, glm::value_ptr(P));
        glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, glm::value_ptr(V));
    });

    /*Distant Heat shader*/
    auto distanceShader = ResourceManager::Factory::createShader(HEAT_DISTANCE_SHADER, "res/shaders/distanceHazev.glsl", "res/shaders/distanceHazef5.glsl");
    distanceShader->setAttribLocation("inPosition", VERTICES__ATTR);
    distanceShader->link();
    distanceShader->use();
    renderderTextureLoc = distanceShader->getUniformLocation("renderedTexture");
    noiseLoc = distanceShader->getUniformLocation("noiseTexture");
    glUniform1i(renderderTextureLoc, 0);
    glUniform1i(noiseLoc, 1);
    MVPLocation = distanceShader->getUniformLocation("MVP");
    distanceShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P) {
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(P * V * M));
    });

    /*Heat reflection shader*/
    auto heatRShader = ResourceManager::Factory::createShader(HEAT_SPOT_REFLECTION_SHADER, "res/shaders/heatReflectionV.glsl", "res/shaders/heatReflectionF.glsl");
    heatRShader->setAttribLocation("vertex", VERTICES__ATTR);
    heatRShader->link();
    heatRShader->use();
    ModelLocation = heatRShader->getUniformLocation("Model");
    ViewLocation = heatRShader->getUniformLocation("View");
    ProjectionLocation = heatRShader->getUniformLocation("Projection");
    textureLoc = glGetUniformLocation(heatRShader->getShader(), "map");
    GLint mirrorLoc = glGetUniformLocation(heatRShader->getShader(), "mirror");
    glUniform1i(textureLoc, 0);
    glUniform1i(mirrorLoc, 1);
    heatRShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P) {
        glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(M));
        glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, glm::value_ptr(V));
        glUniformMatrix4fv(ProjectionLocation, 1, GL_FALSE, glm::value_ptr(P));
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