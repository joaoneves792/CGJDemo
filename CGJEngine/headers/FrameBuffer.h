//
// Created by joao on 11/19/17.
//

#ifndef CGJDEMO_FRAMEBUFFER_H
#define CGJDEMO_FRAMEBUFFER_H

#include <GL/glew.h>
#include "Texture.h"

class FrameBuffer{
protected:
    GLuint _frameBuffer;
    int _width;
    int _height;
public:
    void bind();
    void unbind();
    virtual void blit();
    virtual void resize(int x, int y) = 0;
    virtual ~FrameBuffer() = default;
};




#endif //CGJDEMO_FRAMEBUFFER_H
