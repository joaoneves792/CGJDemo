//
// Created by joao on 11/21/17.
//

#include "CGJengine.h"
#include "shaders.h"
#include "ResourceNames.h"


void loadShaders(){
    int MVPLocation;
    int ModelLocation;
    int ViewLocation;
    int NormalLocation;
    int lightPositionLocation;
    int lightsEnabledLocation;
    int lightColorLocation;
    int lightConeLocation;
    int lightAttenuationLocation;

    /*H3D Shader*/
    auto h3dShader = ResourceFactory::createShader(H3D_SHADER, "res/shaders/vert.glsl", "res/shaders/frag.glsl");
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
    h3dShader->setMVPFunction([=](Mat4 M, Mat4 V, Mat4 P){
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, (P*V*M).transpose());
        glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, M.transpose());
        glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, V.transpose());
        glUniformMatrix4fv(NormalLocation, 1, GL_FALSE, (V*M).inverse()); //Transpose is implicit
    });

    lightPositionLocation = h3dShader->getUniformLocation("lightPosition_worldspace[0]");
    lightsEnabledLocation = h3dShader->getUniformLocation("lightsEnabled[0]");
    lightColorLocation = h3dShader->getUniformLocation("lightColor[0]");
    lightConeLocation = h3dShader->getUniformLocation("lightCone[0]");
    lightAttenuationLocation = h3dShader->getUniformLocation("lightAttenuation[0]");
    LightsManager::getInstance()->registerShader(h3dShader, [=](float* color, float* position, float* cone,
                                                                float* attenuation, int enabled, int i){
        glUniform1iv(lightsEnabledLocation+i, 1, &enabled);
        if(enabled) {
            glUniform3fv(lightColorLocation + i, 1, color);
            glUniform4fv(lightConeLocation + i, 1, cone);
            glUniform4fv(lightAttenuationLocation + i, 1, attenuation);
            glUniform3fv(lightPositionLocation + i, 1, position);
        }
    });



    /*Sky Shader*/
    auto skyShader = ResourceFactory::createShader(SKY_SHADER, "res/shaders/skyv.glsl", "res/shaders/skyf.glsl");
    skyShader->setAttribLocation("inPosition", VERTICES__ATTR);
    skyShader->setAttribLocation("inTexCoord", TEXCOORDS__ATTR);
    skyShader->link();
    MVPLocation = skyShader->getUniformLocation("MVP");
    skyShader->setMVPFunction([=](Mat4 M, Mat4 V, Mat4 P) {
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, (P * V * M).transpose());
    });

    /*Quad shader*/
    auto quadShader = ResourceFactory::createShader(QUAD_SHADER, "res/shaders/quadv.glsl", "res/shaders/quadf.glsl");
    quadShader->setAttribLocation("inPosition", VERTICES__ATTR);
    quadShader->link();
    MVPLocation = quadShader->getUniformLocation("MVP");
    quadShader->setMVPFunction([=](Mat4 M, Mat4 V, Mat4 P) {
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, (P * V * M).transpose());
    });

    /*2D shader*/
    auto twoDShader = ResourceFactory::createShader(TWO_D_SHADER, "res/shaders/2Dv.glsl", "res/shaders/2Df.glsl");
    twoDShader->setAttribLocation("inPosition", VERTICES__ATTR);
    twoDShader->link();
    MVPLocation = twoDShader->getUniformLocation("MVP");
    twoDShader->setMVPFunction([=](Mat4 M, Mat4 V, Mat4 P) {
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, (P * V * M).transpose());
    });

}