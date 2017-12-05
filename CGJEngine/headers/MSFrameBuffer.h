//
// Created by joao on 12/5/17.
//

#ifndef CGJDEMO_MSFRAMEBUFFER_H
#define CGJDEMO_MSFRAMEBUFFER_H


#include <GL/glew.h>
#include "FrameBuffer.h"
#include "Texture.h"
#include "TextureFrameBuffer.h"

class MSFrameBuffer : public FrameBuffer{
private:
    GLuint _depthBuffer;
    GLuint _samples;
    Texture* _texture;

public:
    MSFrameBuffer(int x, int y, int msaaLevel);
    virtual ~MSFrameBuffer();
    void resize(int x, int y) override ;
    using FrameBuffer::blit;
    void blit(TextureFrameBuffer* texFBO);
private:
    void initializeNewFrameBuffer(int x, int y);
    void destroy();
};


#endif //CGJDEMO_MSFRAMEBUFFER_H
