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

    /*H3D Shader*/
    auto h3dShader = ResourceFactory::createShader(H3D_SHADER, "res/h3dv.glsl", "res/frag.glsl");
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
    h3dShader->setMVPFunction([=](Mat4 M, Mat4 V, Mat4 P){
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, (P*V*M).transpose());
        glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, M.transpose());
        glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, V.transpose());
    });

    /*Sky Shader*/
    auto skyShader = ResourceFactory::createShader(SKY_SHADER, "res/skyv.glsl", "res/skyf.glsl");
    skyShader->setAttribLocation("inPosition", VERTICES__ATTR);
    skyShader->setAttribLocation("inTexCoord", TEXCOORDS__ATTR);
    skyShader->link();
    MVPLocation = skyShader->getUniformLocation("MVP");
    skyShader->setMVPFunction([=](Mat4 M, Mat4 V, Mat4 P) {
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, (P * V * M).transpose());
    });

    /*Quad shader*/
    auto quadShader = ResourceFactory::createShader(QUAD_SHADER, "res/quadv.glsl", "res/quadf.glsl");
    quadShader->setAttribLocation("inPosition", VERTICES__ATTR);
    quadShader->link();
    MVPLocation = quadShader->getUniformLocation("MVP");
    quadShader->setMVPFunction([=](Mat4 M, Mat4 V, Mat4 P) {
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, (P * V * M).transpose());
    });

}