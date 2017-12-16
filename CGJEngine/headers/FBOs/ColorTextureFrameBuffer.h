//
// Created by joao on 12/5/17.
//

#ifndef CGJDEMO_TEXTUREFRAMEBUFFER_H
#define CGJDEMO_TEXTUREFRAMEBUFFER_H

#include "Texture.h"
#include "FBOs/FrameBuffer.h"

class ColorTextureFrameBuffer : public FrameBuffer{
public:
    ColorTextureFrameBuffer(int x, int y);
    ~ColorTextureFrameBuffer() override ;
    void resize(int x, int y) override ;
    void bindTexture() override;
    Texture* getTexture();
    using FrameBuffer::blit;
private:
    void initializeNewFrameBuffer(int x, int y);
    void destroy();

    friend class MSFrameBuffer;
};


#endif //CGJDEMO_TEXTUREFRAMEBUFFER_H
