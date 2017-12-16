//
// Created by joao on 12/5/17.
//

#ifndef CGJDEMO_MSFRAMEBUFFER_H
#define CGJDEMO_MSFRAMEBUFFER_H


#include <GL/glew.h>
#include "FrameBuffer.h"
#include "Texture.h"
#include "ColorTextureFrameBuffer.h"
#include "DepthTextureFrameBuffer.h"

class MSFrameBuffer : public FrameBuffer{
private:
    GLuint _samples;
public:
    MSFrameBuffer(int x, int y, int msaaLevel);
    ~MSFrameBuffer() override;
    void resize(int x, int y) override ;
    using FrameBuffer::blit;
    void blit(ColorTextureFrameBuffer* texFBO);
    void blitDepth(DepthTextureFrameBuffer* texFBO);
private:
    void initializeNewFrameBuffer(int x, int y);
    void destroy();
};


#endif //CGJDEMO_MSFRAMEBUFFER_H
