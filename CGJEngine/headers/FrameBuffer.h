//
// Created by joao on 11/19/17.
//

#ifndef CGJDEMO_FRAMEBUFFER_H
#define CGJDEMO_FRAMEBUFFER_H

#include <GL/glew.h>
#include "Texture.h"

class FrameBuffer {
private:
    GLuint _frameBuffer;
    GLuint _depthRenderBuffer;
    Texture* _texture;

    int _width, _height;
public:
    FrameBuffer(int x, int y);
    FrameBuffer(FrameBuffer* otherFrameBuffer);
    ~FrameBuffer();
    void resize(int x, int y);
    void bind();
    void unbind();
    void bindTexture();
    Texture* getTexture();

    void copyFrameBuffer(FrameBuffer* otherFrameBuffer);
private:
    void initializeNewFrameBuffer(int x, int y);
    void destroy();
};


#endif //CGJDEMO_FRAMEBUFFER_H
