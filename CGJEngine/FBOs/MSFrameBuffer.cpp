//
// Created by joao on 12/5/17.
//

#include <cstdlib>
#include <iostream>
#include "FBOs/MSFrameBuffer.h"

MSFrameBuffer::MSFrameBuffer(int x, int y, int msaaLevel) {
    _width = x;
    _height = y;
    _samples = (GLuint)msaaLevel;
    _colorBuffer = new Texture();
    _depthStencilBuffer = new Texture();
    initializeNewFrameBuffer(x, y);
}

void MSFrameBuffer::initializeNewFrameBuffer(int x, int y) {
    _width = x;
    _height = y;

    glGenFramebuffers(1, &_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

    GLuint colorBuffer;
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colorBuffer);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _samples, _colorInternalFormat, _width, _height, GL_TRUE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, colorBuffer, 0);

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
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Failed to create FrameBuffer" << std::endl;
        exit(EXIT_FAILURE);
    }
    //glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    _colorBuffer->changeTexture(colorBuffer);
    _depthStencilBuffer->changeTexture(depthBuffer);
}

void MSFrameBuffer::resize(int x, int y) {
    _width = x;
    _height = y;

    destroy();
    _colorBuffer = new Texture();
    _depthStencilBuffer = new Texture();
    initializeNewFrameBuffer(_width, _height);

}

void MSFrameBuffer::destroy(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    _colorBuffer->destroyTexture();
    delete _colorBuffer;
    _depthStencilBuffer->destroyTexture();
    delete _depthStencilBuffer;
    glDeleteFramebuffers(1, &_frameBuffer);
}


void MSFrameBuffer::blit(ColorTextureFrameBuffer *texFBO) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, texFBO->_frameBuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    //source and destination width and height must match on a MS FBO blit
    glBlitFramebuffer(0, 0, _width, _height,
                      0, 0, texFBO->_width, texFBO->_height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
}

void MSFrameBuffer::blitDepth(DepthTextureFrameBuffer *texFBO) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, texFBO->_frameBuffer);

    //source and destination width and height must match on a MS FBO blit
    glBlitFramebuffer(0, 0, _width, _height,
                      0, 0, texFBO->_width, texFBO->_height,
                      GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);

}

MSFrameBuffer::~MSFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    /*Parent destructor should take care of the rest*/
}