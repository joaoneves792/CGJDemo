//
// Created by joao on 12/5/17.
//

#ifndef CGJDEMO_DOUBLECOLORMSFRAMEBUFFER_H
#define CGJDEMO_DOUBLECOLORMSFRAMEBUFFER_H


#include <GL/glew.h>
#include "FrameBuffer.h"
#include "Texture.h"
#include "ColorTextureFrameBuffer.h"
#include "DepthTextureFrameBuffer.h"

class DoubleColorMSFrameBuffer : public FrameBuffer{
private:
    Texture* _secondColorBuffer;
    GLuint _samples;
public:
    DoubleColorMSFrameBuffer(int x, int y, int msaaLevel);
    ~DoubleColorMSFrameBuffer() override;
    void resize(int x, int y) override ;
    using FrameBuffer::blit;
    void blit(FrameBuffer* destFBO);
    //void blit(DoubleColorFrameBuffer* destFBO);
    void blitColor(ColorTextureFrameBuffer* destFBO);
    void blitDepth(DepthTextureFrameBuffer* destFBO);
    void blitSecondColor(ColorTextureFrameBuffer* destFBO);
private:
    void initializeNewFrameBuffer(int x, int y);
    void destroy();
    friend class FrameBuffer;
};


#endif //CGJDEMO_MSFRAMEBUFFER_H
