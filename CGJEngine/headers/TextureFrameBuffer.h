//
// Created by joao on 12/5/17.
//

#ifndef CGJDEMO_TEXTUREFRAMEBUFFER_H
#define CGJDEMO_TEXTUREFRAMEBUFFER_H

#include "Texture.h"
#include "FrameBuffer.h"

class TextureFrameBuffer : public FrameBuffer{
public:
    TextureFrameBuffer(int x, int y);
    ~TextureFrameBuffer() override ;
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
