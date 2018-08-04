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
    Texture* _depthStencilBuffer;
    GLuint _colorInternalFormat = GL_RGBA8;
    GLuint _normalInternalFormat = GL_RGBA16F;
    GLuint _depthInternalFormat = GL_DEPTH_COMPONENT32;
    int _width;
    int _height;
    Texture* _colorBuffer;
    friend class ColorTextureFrameBuffer;
    friend class DepthTextureFrameBuffer;
    friend class MSFrameBuffer;
    friend class DoubleColorMSFrameBuffer;
    friend class GFrameBuffer;
public:
    FrameBuffer();
    FrameBuffer(int x, int y);
    void setInternalFormats(GLuint color, GLuint normal, GLuint depth);
    void bind();
    void unbind();
    virtual void bindTexture(); //This doesnt work with MSFramebuffers!
    virtual void blit();
    virtual void resize(int x, int y);
    virtual ~FrameBuffer();

private:
    void initializeNewFrameBuffer(int x, int y);
    void destroy();
};




#endif //CGJDEMO_FRAMEBUFFER_H
