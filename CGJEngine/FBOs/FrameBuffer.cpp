//
// Created by joao on 11/19/17.
//
#include <GL/glew.h>
#include <cstdlib>
#include <iostream>

#include "FBOs/FrameBuffer.h"

void FrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    glViewport(0,0,_width,_height);
}

void FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::blit() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer);
    glDrawBuffer(GL_BACK);
    glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
}

FrameBuffer::FrameBuffer(int x, int y) {
    _width = x;
    _height = y;
    _colorBuffer = new Texture();
    _depthStencilBuffer = new Texture();
    initializeNewFrameBuffer(x, y);
}

void FrameBuffer::initializeNewFrameBuffer(int x, int y) {
    _width = x;
    _height = y;

    glGenFramebuffers(1, &_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

    GLuint colorBuffer;
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    /*glGenRenderbuffers(1, &_depthStencilBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthStencilBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthStencilBuffer);*/

    GLuint depthBuffer;
    glGenTextures(1, &depthBuffer);
    glBindTexture(GL_TEXTURE_2D, depthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glGenFramebuffers(1, &_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorBuffer, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthBuffer, 0);


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

void FrameBuffer::bindTexture() {
    _colorBuffer->bind();
}

void FrameBuffer::destroy() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    if(_colorBuffer) {
        _colorBuffer->destroyTexture();
        delete _colorBuffer;
    }
    if(_depthStencilBuffer) {
        _depthStencilBuffer->destroyTexture();
        delete _depthStencilBuffer;
    }
    glDeleteFramebuffers(1, &_frameBuffer);
}

FrameBuffer::FrameBuffer() {
    _colorBuffer = nullptr;
    _depthStencilBuffer = nullptr;
}

FrameBuffer::~FrameBuffer() {
    destroy();
}

void FrameBuffer::resize(int x, int y) {
    destroy();
    _colorBuffer = new Texture();
    _depthStencilBuffer = new Texture();
    initializeNewFrameBuffer(x, y);
}