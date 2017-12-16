//
// Created by joao on 12/5/17.
//

#include <cstdlib>
#include <iostream>
#include "FBOs/DoubleColorMSFrameBuffer.h"

DoubleColorMSFrameBuffer::DoubleColorMSFrameBuffer(int x, int y, int msaaLevel) {
    _width = x;
    _height = y;
    _samples = (GLuint)msaaLevel;
    _colorBuffer = new Texture();
    _depthStencilBuffer = new Texture();
    _secondColorBuffer = new Texture();
    initializeNewFrameBuffer(x, y);
}

void DoubleColorMSFrameBuffer::initializeNewFrameBuffer(int x, int y) {
    _width = x;
    _height = y;

    glGenFramebuffers(1, &_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

    GLuint colorBuffer;
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colorBuffer);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _samples, GL_RGBA8, _width, _height, GL_TRUE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, colorBuffer, 0);

    GLuint secondColorBuffer;
    glGenTextures(1, &secondColorBuffer);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, secondColorBuffer);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _samples, GL_RGBA8, _width, _height, GL_TRUE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, secondColorBuffer, 0);

    /*glGenRenderbuffers(1, &_depthStencilBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthStencilBuffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, _samples, GL_DEPTH24_STENCIL8, _width, _height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthStencilBuffer);*/

    GLuint depthBuffer;
    glGenTextures(1, &depthBuffer);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, depthBuffer);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _samples, GL_DEPTH24_STENCIL8, _width, _height, GL_TRUE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, depthBuffer, 0);

    // Set the list of draw buffers.
    GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, DrawBuffers); // "1" is the size of DrawBuffers

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Failed to create FrameBuffer" << std::endl;
        exit(EXIT_FAILURE);
    }
    //glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    _colorBuffer->changeTexture(colorBuffer);
    _secondColorBuffer->changeTexture(secondColorBuffer);
    _depthStencilBuffer->changeTexture(depthBuffer);
}

void DoubleColorMSFrameBuffer::resize(int x, int y) {
    _width = x;
    _height = y;

    destroy();
    _colorBuffer = new Texture();
    _secondColorBuffer = new Texture();
    _depthStencilBuffer = new Texture();
    initializeNewFrameBuffer(_width, _height);

}

void DoubleColorMSFrameBuffer::destroy(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    _colorBuffer->destroyTexture();
    delete _colorBuffer;
    _secondColorBuffer->destroyTexture();
    delete _secondColorBuffer;
    _depthStencilBuffer->destroyTexture();
    delete _depthStencilBuffer;
    glDeleteFramebuffers(1, &_frameBuffer);
}


void DoubleColorMSFrameBuffer::blit(FrameBuffer *destFBO) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destFBO->_frameBuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    //source and destination width and height must match on a MS FBO blit
    glBlitFramebuffer(0, 0, _width, _height,
                      0, 0, destFBO->_width, destFBO->_height,
                      GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
}

void DoubleColorMSFrameBuffer::blitColor(ColorTextureFrameBuffer *destFBO) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destFBO->_frameBuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    //source and destination width and height must match on a MS FBO blit
    glBlitFramebuffer(0, 0, _width, _height,
                      0, 0, destFBO->_width, destFBO->_height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
}


void DoubleColorMSFrameBuffer::blitSecondColor(ColorTextureFrameBuffer *destFBO) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT1);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destFBO->_frameBuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    //source and destination width and height must match on a MS FBO blit
    glBlitFramebuffer(0, 0, _width, _height,
                      0, 0, destFBO->_width, destFBO->_height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);

}

void DoubleColorMSFrameBuffer::blitDepth(DepthTextureFrameBuffer *destFBO) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destFBO->_frameBuffer);

    //source and destination width and height must match on a MS FBO blit
    glBlitFramebuffer(0, 0, _width, _height,
                      0, 0, destFBO->_width, destFBO->_height,
                      GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);

}

DoubleColorMSFrameBuffer::~DoubleColorMSFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    _secondColorBuffer->destroyTexture();
    delete _secondColorBuffer;
    /*Parent destructor should take care of the rest*/
}

