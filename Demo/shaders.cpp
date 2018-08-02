//
// Created by joao on 11/21/17.
//

#include "CGJengine.h"
#include "shaders.h"
#include "constants.h"


void loadShaders(){
    /*H3D Shader*/
    auto Gh3dShader = ResourceManager::Factory::createShader(H3D_SHADER, "res/shaders/Gh3dv.glsl", "res/shaders/Gh3df.glsl");
    Gh3dShader->setAttribLocation("inPositon", VERTICES__ATTR);
    Gh3dShader->setAttribLocation("inNormal", NORMALS__ATTR);
    Gh3dShader->setAttribLocation("inTexCoord", TEXCOORDS__ATTR);
    Gh3dShader->setAttribLocation("inJointIndex", BONEINDICES__ATTR);
    Gh3dShader->setAttribLocation("inJointWeight", BONEWEIGHTS__ATTR);
    Gh3dShader->setFragOutputLocation("G_output", 0);
    Gh3dShader->link();

    int MVPLocation = Gh3dShader->getUniformLocation("MVP");
    int ModelLocation = Gh3dShader->getUniformLocation("Model");
    int ViewLocation = Gh3dShader->getUniformLocation("View");
    int NormalLocation = Gh3dShader->getUniformLocation("NormalMatrix");
    GLint textureLoc = Gh3dShader->getUniformLocation("texture_sampler");
    GLint environmentLoc = Gh3dShader->getUniformLocation("environment");
    Gh3dShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P){
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(P*V*M));
        glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(M));
        glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, glm::value_ptr(V));
        glUniformMatrix4fv(NormalLocation, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(V*M))));
    });
    Gh3dShader->use();
    glUniform1i(textureLoc, TEXTURE_SLOT);
    glUniform1i(environmentLoc, ENVIRONMENT_SLOT);

    /*Asphalt Shader*/
    auto asphaltShader = ResourceManager::Factory::createShader(ASPHALT_SHADER, "res/shaders/Gh3dv.glsl", "res/shaders/Gasphaltf.glsl");
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
    int ProjectionLocation = heatShader->getUniformLocation("Projection");
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

    /*Shadow Shader*/
    /*auto shadowMapShader = ResourceManager::Factory::createShader(SHADOW_SHADER, "res/shaders/shadowv.glsl", "res/shaders/shadowf.glsl");
    shadowMapShader->setAttribLocation("positon", VERTICES__ATTR);
    //shadowMapShader->setAttribLocation("inJointIndex", BONEINDICES__ATTR);
    //shadowMapShader->setAttribLocation("inJointWeight", BONEWEIGHTS__ATTR);
    //shadowMapShader->setFragOutputLocation("fragmentdepth", 0);
    shadowMapShader->link();*/


    /*SSAO shader*/
    auto ssaoShader = ResourceManager::Factory::createShader(SSAO_SHADER, "res/shaders/quadViewRayv.glsl", "res/shaders/ssaof.glsl");
    ssaoShader->setAttribLocation("inPosition", VERTICES__ATTR);
    ssaoShader->link();
    ssaoShader->use();
    GLint depthBufferLoc = ssaoShader->getUniformLocation("depthBuffer");
    GLint normalsLoc = ssaoShader->getUniformLocation("normals");
    noiseLoc = ssaoShader->getUniformLocation("noise");
    MVPLocation = ssaoShader->getUniformLocation("MVP");
    glUniform1i(depthBufferLoc, 0);
    glUniform1i(noiseLoc, 1);
    glUniform1i(normalsLoc, 2);
    ssaoShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P) {
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(P * V * M));

    });

    /*SSAO blur shader*/
    auto ssaoBlurShader = ResourceManager::Factory::createShader(SSAO_BLUR_SHADER, "res/shaders/quadv.glsl", "res/shaders/ssaoBlurf.glsl");
    ssaoBlurShader->setAttribLocation("inPosition", VERTICES__ATTR);
    ssaoBlurShader->link();
    ssaoBlurShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P) {
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(P * V * M));
    });

    /*General purpose blit shader*/
    auto blitShader = ResourceManager::Factory::createShader(BLIT_SHADER, "res/shaders/quadv.glsl", "res/shaders/blitf.glsl");
    blitShader->setAttribLocation("inPosition", VERTICES__ATTR);
    blitShader->link();
    blitShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P) {
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(P * V * M));
    });

    /*FXAA Shader*/
    auto fxaaShader = ResourceManager::Factory::createShader(FXAA_SHADER, "res/shaders/quadv.glsl", "res/shaders/fxaaf.glsl");
    fxaaShader->setAttribLocation("inPosition", VERTICES__ATTR);
    fxaaShader->link();
    fxaaShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P) {
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(P * V * M));
    });



    /*Lighting Shader*/
    auto lightingShader = ResourceManager::Factory::createShader(LIGHTING_SHADER, "res/shaders/quadViewRayv.glsl", "res/shaders/lightingf.glsl");
    lightingShader->setAttribLocation("inPosition", VERTICES__ATTR);
    lightingShader->link();
    lightingShader->use();
    GLint diffuseLoc = lightingShader->getUniformLocation("diffuse");
    GLint specularLoc = lightingShader->getUniformLocation("specular");
    GLint depthLoc = lightingShader->getUniformLocation("depth");
    GLint normalLoc = lightingShader->getUniformLocation("normals");
    MVPLocation = lightingShader->getUniformLocation("MVP");
    glUniform1i(diffuseLoc, 0);
    glUniform1i(specularLoc, 1);
    glUniform1i(depthLoc, 2);
    glUniform1i(normalLoc, 3);
    lightingShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P) {
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(P * V * M));

    });
    int lightPositionLocation = lightingShader->getUniformLocation("lightPosition_viewspace[0]");
    int lightsEnabledLocation = lightingShader->getUniformLocation("lightsEnabled[0]");
    int lightColorLocation = lightingShader->getUniformLocation("lightColor[0]");
    int lightConeLocation = lightingShader->getUniformLocation("lightCone_viewspace[0]");
    int lightAttenuationLocation = lightingShader->getUniformLocation("lightAttenuation[0]");
    LightsManager::getInstance()->registerShader(lightingShader, [=](Vec3 color, Vec3 position, Vec4 cone,
                                                                Vec4 attenuation, int enabled, int i){
        glUniform1iv(lightsEnabledLocation+i, 1, &enabled);
        if(enabled) {
            Mat4 View = ResourceManager::getInstance()->getScene(SCENE)->getViewMatrix();
            Vec3 position_viewspace = View * Vec4(position, 1.0f);
            Vec4 cone_viewspace = Vec4(Mat3(View) * Vec3(cone), cone[3]);
            glUniform3fv(lightColorLocation + i, 1, glm::value_ptr(color));
            glUniform4fv(lightConeLocation + i, 1, glm::value_ptr(cone_viewspace));
            glUniform4fv(lightAttenuationLocation + i, 1, glm::value_ptr(attenuation));
            glUniform3fv(lightPositionLocation + i, 1, glm::value_ptr(position_viewspace));
        }
    });

    /*Ambient shading*/
    auto ambientShader = ResourceManager::Factory::createShader(AMBIENT_BLEND_SHADER, "res/shaders/quadViewRayv.glsl", "res/shaders/ambientBlendf.glsl");
    ambientShader->setAttribLocation("inPosition", VERTICES__ATTR);
    ambientShader->link();
    ambientShader->use();
    GLint frameLoc = ambientShader->getUniformLocation("frame");
    GLint ambientLoc = ambientShader->getUniformLocation("ambient");
    GLint occlusionLoc = ambientShader->getUniformLocation("ambientOcclusion");
    depthLoc = ambientShader->getUniformLocation("depth");
    GLint shadowLoc = ambientShader->getUniformLocation("shadow");
    MVPLocation = ambientShader->getUniformLocation("MVP");
    glUniform1i(frameLoc, 0);
    glUniform1i(ambientLoc, 1);
    glUniform1i(occlusionLoc, 2);
    glUniform1i(depthLoc, 3);
    glUniform1i(shadowLoc, 4);
    ambientShader->setMVPFunction([=](const Mat4& M, const Mat4& V, const Mat4& P) {
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(P * V * M));

    });
}