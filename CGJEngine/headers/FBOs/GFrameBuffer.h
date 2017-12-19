//
// Created by joao on 12/5/17.
//

#ifndef CGJDEMO_GFRAMEBUFFER_H
#define CGJDEMO_GFRAMEBUFFER_H


#include <GL/glew.h>
#include "FrameBuffer.h"
#include "Texture.h"
#include "ColorTextureFrameBuffer.h"
#include "DepthTextureFrameBuffer.h"

class GFrameBuffer : public FrameBuffer{
private:
    Texture* _diffuseBuffer;
    Texture* _normalBuffer;
    Texture* _depthBuffer;
    Texture* _specularBuffer;
    Texture* _ambientBuffer;
    Texture* _particleBuffer;
public:
    GFrameBuffer(int x, int y);
    ~GFrameBuffer() override;
    void resize(int x, int y) override ;
    void blitDiffuse(ColorTextureFrameBuffer *destFBO);
    void blitDepth(DepthTextureFrameBuffer* destFBO);
    void blitNormal(ColorTextureFrameBuffer* destFBO);
    void blitSpecular(ColorTextureFrameBuffer *destFBO);
    void blitAmbient(ColorTextureFrameBuffer *destFBO);
    void bindDiffuse();
    void bindNormals();
    void bindDepth();
    void bindSpecular();
    void bindAmbient();
    void bindParticles();
private:
    void initializeNewFrameBuffer(int x, int y);
    void destroy();
    friend class FrameBuffer;
};


#endif //CGJDEMO_GFRAMEBUFFER_H
