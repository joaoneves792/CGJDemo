//
// Created by joao on 12/5/17.
//

#ifndef CGJDEMO_DEPTHTEXTUREFRAMEBUFFER_H
#define CGJDEMO_DEPTHTEXTUREFRAMEBUFFER_H

#include "Texture.h"
#include "FBOs/FrameBuffer.h"

class DepthTextureFrameBuffer : public FrameBuffer{
public:
    DepthTextureFrameBuffer(int x, int y);
    ~DepthTextureFrameBuffer() override ;
    void resize(int x, int y) override ;
    void bindTexture() override;
    Texture* getTexture();
private:
    void initializeNewFrameBuffer(int x, int y);
    void destroy();

    friend class MSFrameBuffer;
};


#endif //CGJDEMO_TEXTUREFRAMEBUFFER_H
