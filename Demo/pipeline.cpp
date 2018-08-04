//
// Created by joao on 12/19/17.
//

#include <iostream>
#include "CGJengine.h"
#include "constants.h"
#include "pipeline.h"

/*
 * Rendering pipeline
 * There are 4 FBOs
 * The main FBO is a G-Buffer and all meshes on the scene are drawn to it
 * The remaining FBOs are temporary color buffers to store intermediate shading results
 * Steps:
 *  -> Draw scene to mainFBO
 *  -> Extract SSAO occlusion factor
 *  -> Perform Screen Space lighting
 *  -> Blend ambient and occlusion
 *  -> Draw Regular particles
 *  -> Draw Post-FX particles
 *  -> Perform FXAA
 *  -> Draw HUD
 */

void setupPipeline(){
    ResourceManager* rm = ResourceManager::getInstance();
    QuadMesh* quad = (QuadMesh*)rm->getMesh(QUAD);
    SceneGraph* scene = rm->getScene(SCENE);

    /*Create the framebuffers*/
    ResourceManager::Factory::createGFrameBuffer(MAIN_FBO, WIN_X, WIN_Y);
    ResourceManager::Factory::createColorTextureFrameBuffer(SIDE_FBO1, WIN_X, WIN_Y);
    ResourceManager::Factory::createColorTextureFrameBuffer(SIDE_FBO2, WIN_X, WIN_Y);
    ResourceManager::Factory::createColorTextureFrameBuffer(SIDE_FBO3, WIN_X, WIN_Y);
    ResourceManager::Factory::createDepthTextureFrameBuffer(SHADOW_FBO, WIN_X, WIN_Y);

    rm->getFrameBuffer(MAIN_FBO)->setInternalFormats(GL_RGBA16F, GL_RGBA16F, GL_DEPTH_COMPONENT32);
    rm->getFrameBuffer(SIDE_FBO1)->setInternalFormats(GL_RGBA16F, GL_RGBA16F, GL_DEPTH_COMPONENT32);
    rm->getFrameBuffer(SIDE_FBO2)->setInternalFormats(GL_RGBA16F, GL_RGBA16F, GL_DEPTH_COMPONENT32);
    rm->getFrameBuffer(SIDE_FBO3)->setInternalFormats(GL_RGBA16F, GL_RGBA16F, GL_DEPTH_COMPONENT32);

    /*Create the ortho camera*/
    auto finalCamera = ResourceManager::Factory::createHUDCamera(ORTHO_CAM, -1, 1, 1, -1, 0, 1, true);

    /*Create the pipeline scenegraph*/
    auto renderPipeline = ResourceManager::Factory::createScene(PIPELINE, finalCamera);
    renderPipeline->translate(0.0f, 0.0f, -0.2f);

    /*SSAO Stage*/
    auto ssaoShader = rm->getShader(SSAO_SHADER);
    auto ssao = new SceneNode(SSAO, quad, ssaoShader);
    auto ssaoNoise = new Texture();
    ssaoNoise->generateRandom(4);
    rm->addTexture("ssaoNoise", ssaoNoise);
    GLint PLoc = ssaoShader->getUniformLocation("P");
    GLint inversePLoc = ssaoShader->getUniformLocation("inverseP");
    ssao->setProcessingLevel(SSAO_LEVEL);
    ssao->setPreDraw([=](){
        glActiveTexture(GL_TEXTURE1);
        ssaoNoise->bind();
        ssaoShader->use();
        glUniformMatrix4fv(inversePLoc, 1, GL_FALSE, glm::value_ptr(scene->getCamera()->getInverseProjection()));
        glUniformMatrix4fv(PLoc, 1, GL_FALSE, glm::value_ptr(scene->getProjectionMatrix()));
    });
    renderPipeline->addChild(ssao);

    /*SSAO Blurring Stage*/
    auto ssaoBlur = new SceneNode(SSAO_BLUR, quad, rm->getShader(SSAO_BLUR_SHADER));
    ssaoBlur->setProcessingLevel(SSAO_BLUR_LEVEL);
    renderPipeline->addChild(ssaoBlur);

    /*Lighting stage*/
    auto lightingShader = rm->getShader(LIGHTING_SHADER);
    auto lighting = new SceneNode(LIGHTING, quad, lightingShader);
    GLint ViewLoc = lightingShader->getUniformLocation("View");
    GLint ProjectionLoc = lightingShader->getUniformLocation("Projection");
    inversePLoc = lightingShader->getUniformLocation("inverseP");
    lighting->setProcessingLevel(LIGHTS_LEVEL);
    lighting->setPreDraw([=](){
        glUniformMatrix4fv(inversePLoc, 1, GL_FALSE, glm::value_ptr(scene->getCamera()->getInverseProjection()));
        glUniformMatrix4fv(ViewLoc, 1, GL_FALSE, glm::value_ptr(scene->getViewMatrix()));
        glUniformMatrix4fv(ProjectionLoc, 1, GL_FALSE, glm::value_ptr(scene->getProjectionMatrix()));
    });
    renderPipeline->addChild(lighting);

    /*Ambient blending stage (extended lighting)*/
    auto ambientBlend = new SceneNode(AMBIENT, quad, rm->getShader(AMBIENT_BLEND_SHADER));
    auto ambientShader = rm->getShader(AMBIENT_BLEND_SHADER);
    ProjectionLoc = ambientShader->getUniformLocation("Projection");
    inversePLoc = ambientShader->getUniformLocation("inverseP");
    Mat4 biasMatrix(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0);
    GLint DepthBiasMVPLoc = ambientShader->getUniformLocation("depthBiasMVP");
    ambientBlend->setProcessingLevel(AMBIENT_LEVEL);
    auto shadowCamera = rm->getCamera(SHADOW_CAMERA);
    ambientBlend->setPreDraw([=](){
        glUniformMatrix4fv(inversePLoc, 1, GL_FALSE, glm::value_ptr(scene->getCamera()->getInverseProjection()));
        glUniformMatrix4fv(ProjectionLoc, 1, GL_FALSE, glm::value_ptr(scene->getProjectionMatrix()));
        Mat4 bias = biasMatrix * shadowCamera->getProjectionMatrix() *
                    shadowCamera->getViewMatrix() *
                    glm::inverse(scene->getViewMatrix());
        glUniformMatrix4fv(DepthBiasMVPLoc, 1, GL_FALSE, glm::value_ptr(bias));
    });
    renderPipeline->addChild(ambientBlend);


    /*General purpose blitting stage*/
    auto blit = new SceneNode(BLIT, quad, rm->getShader(BLIT_SHADER));
    blit->setProcessingLevel(BLIT_LEVEL);
    blit->setPreDraw([=](){
        glDepthMask(GL_FALSE);
    });
    blit->setPostDraw([=](){
        glDepthMask(GL_TRUE);
    });
    renderPipeline->addChild(blit);

    /*FXAA stage*/
    auto fxaa = new SceneNode(FXAA, quad, rm->getShader(FXAA_SHADER));
    fxaa->setProcessingLevel(FXAA_LEVEL);
    renderPipeline->addChild(fxaa);
}

void executePipeline(){
    static SceneGraph* scene = ResourceManager::getInstance()->getScene(SCENE);
    static SceneGraph* pipeline = ResourceManager::getInstance()->getScene(PIPELINE);
    static SceneGraph* creditsHUD = ResourceManager::getInstance()->getScene(CREDITS_HUD);

    static GFrameBuffer* mainFBO = (GFrameBuffer*)ResourceManager::getInstance()->getFrameBuffer(MAIN_FBO);
    static ColorTextureFrameBuffer* sideBuffer1 = (ColorTextureFrameBuffer*)ResourceManager::getInstance()->getFrameBuffer(SIDE_FBO1);
    static ColorTextureFrameBuffer* sideBuffer2 = (ColorTextureFrameBuffer*)ResourceManager::getInstance()->getFrameBuffer(SIDE_FBO2);
    static ColorTextureFrameBuffer* sideBuffer3 = (ColorTextureFrameBuffer*)ResourceManager::getInstance()->getFrameBuffer(SIDE_FBO3);
    static DepthTextureFrameBuffer* shadowBuffer = (DepthTextureFrameBuffer*)ResourceManager::getInstance()->getFrameBuffer(SHADOW_FBO);

    static Shader* blit = ResourceManager::getInstance()->getShader(BLIT_SHADER);
    static Shader* shadowShader = ResourceManager::getInstance()->getShader(SHADOW_SHADER);

    static GLint renderTargetLoc = blit->getUniformLocation("renderTarget");

    static ParticlePool* particlePool = ResourceManager::getInstance()->getParticlePool(POOL);

    /*Draw scene to G-Buffer*/
    mainFBO->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->draw();
    mainFBO->unbind();

    shadowBuffer->bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    scene->draw(shadowShader);
    shadowBuffer->unbind();

    /*Perform SSAO stage*/
    sideBuffer1->bind();
    glActiveTexture(GL_TEXTURE2);
    mainFBO->bindNormals();
    glActiveTexture(GL_TEXTURE0);
    mainFBO->bindDepth();
    pipeline->draw(SSAO_LEVEL);
    sideBuffer1->unbind();
    sideBuffer2->bind();
    glActiveTexture(GL_TEXTURE0);
    sideBuffer1->bindTexture();
    pipeline->draw(SSAO_BLUR_LEVEL);
    sideBuffer2->unbind();

    /*Perform Lighting stage*/
    LightsManager::getInstance()->uploadLights();
    sideBuffer1->bind();
    glActiveTexture(GL_TEXTURE0);
    mainFBO->bindDiffuse();
    glActiveTexture(GL_TEXTURE1);
    mainFBO->bindSpecular();
    glActiveTexture(GL_TEXTURE2);
    mainFBO->bindDepth();
    glActiveTexture(GL_TEXTURE3);
    mainFBO->bindNormals();
    glActiveTexture(GL_TEXTURE0);
    pipeline->draw(LIGHTS_LEVEL);
    //float data[4];
    //glReadPixels(0, 0, 1, 1, GL_RGBA, GL_FLOAT, &data[0]);
    //std::cout << data[0] << " " << data[1] << " " << data[2] << std::endl;
    sideBuffer1->unbind();

    /*Blend in the ambient and occlusion information*/
    sideBuffer3->bind();
    glActiveTexture(GL_TEXTURE0);
    sideBuffer1->bindTexture();
    glActiveTexture(GL_TEXTURE1);
    mainFBO->bindAmbient();
    glActiveTexture(GL_TEXTURE2);
    sideBuffer2->bindTexture();
    glActiveTexture(GL_TEXTURE3);
    mainFBO->bindDepth();
    glActiveTexture(GL_TEXTURE4);
    shadowBuffer->bindTexture();
    glActiveTexture(GL_TEXTURE0);
    pipeline->draw(AMBIENT_LEVEL);
    sideBuffer3->unbind();

    /*Prepare buffer to draw particles*/
    /**/
    mainFBO->bind();
    blit->use();
    glUniform1i(renderTargetLoc, 4);
    glActiveTexture(GL_TEXTURE0);
    sideBuffer3->bindTexture();
    pipeline->draw(BLIT_LEVEL);
    /**/

    /*Draw the normal particles*/
    /**/
    particlePool->draw(DEFAULT_PARTICLES_LEVEL);
    mainFBO->unbind();
    /**/

    /*Prepare the background for post-FX particles*/
    /**/
    sideBuffer3->bind();
    blit->use();
    glUniform1i(renderTargetLoc, 0);
    glActiveTexture(GL_TEXTURE0);
    mainFBO->bindParticles();
    pipeline->draw(BLIT_LEVEL);
    sideBuffer3->unbind();
    /**/

    /*Draw post-FX particles*/
    /**/
    mainFBO->bind();
    glActiveTexture(GL_TEXTURE0);
    sideBuffer3->bindTexture();
    scene->draw(HEAT_HAZE_LEVEL);
    particlePool->draw(HEAT_HAZE_LEVEL);
    mainFBO->unbind();
    /**/

    /*Apply FXAA and render to screen*/
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    mainFBO->bindParticles();
    //sideBuffer3->bindTexture();
    pipeline->draw(FXAA_LEVEL);


    /* Uncomment for result of SSAO*/
    //sideBuffer2->blit();
    //shadowBuffer->blit();
    /*
    blit->use();
    glUniform1i(renderTargetLoc, 0);
    glActiveTexture(GL_TEXTURE0);
    shadowBuffer->bindTexture();
    pipeline->draw(BLIT_LEVEL);*/

    creditsHUD->draw();
}