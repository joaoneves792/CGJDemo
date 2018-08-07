//
// Created by joao on 12/5/17.
//

#include <iostream>
#include "FBOs/DepthTextureFrameBuffer.h"

DepthTextureFrameBuffer::DepthTextureFrameBuffer(int x, int y) {
    _width = x;
    _height = y;
    _colorBuffer = nullptr;
    _depthStencilBuffer = new Texture();
    initializeNewFrameBuffer(x, y);
}

void DepthTextureFrameBuffer::initializeNewFrameBuffer(int x, int y) {
    _width = x;
    _height = y;

    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    // Give an empty image to OpenGL
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, _depthInternalFormat, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_GREATER);

    glGenFramebuffers(1, &_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Failed to create FrameBuffer" << std::endl;
        exit(EXIT_FAILURE);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    _depthStencilBuffer->changeTexture(texture);
}

void DepthTextureFrameBuffer::destroy(){
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    _depthStencilBuffer->destroyTexture();
    delete _depthStencilBuffer;
    glDeleteFramebuffers(1, &_frameBuffer);
}

void DepthTextureFrameBuffer::resize(int x, int y) {
    _width = x;
    _height = y;

    destroy();
    _depthStencilBuffer = new Texture();
    initializeNewFrameBuffer(x, y);
}

void DepthTextureFrameBuffer::bindTexture() {
    _depthStencilBuffer->bind();
}

Texture* DepthTextureFrameBuffer::getTexture() {
    return _depthStencilBuffer;
}

DepthTextureFrameBuffer::~DepthTextureFrameBuffer() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    /*Parent destructor should take care of the rest*/
}
