//
// Created by joao on 11/19/17.
//
#include <GL/glew.h>
#include <cstdlib>
#include <iostream>

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(int x, int y) {
    initializeNewFrameBuffer(x, y);
}

void FrameBuffer::initializeNewFrameBuffer(int x, int y) {
    this->_width = x;
    this->_height = y;

    glGenFramebuffers(1, &this->_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->_frameBuffer);
    glGenTextures(1, &this->_texture);

    glBindTexture(GL_TEXTURE_2D, this->_texture);

    // Give an empty image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->_width, this->_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glGenRenderbuffers(1, &this->_depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, this->_depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->_width, this->_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->_depthRenderBuffer);


    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->_texture, 0);

    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Failed to create FrameBuffer" << std::endl;
        exit(EXIT_FAILURE);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::resize(int x, int y) {
    destroy();
    initializeNewFrameBuffer(x, y);
}

FrameBuffer::FrameBuffer(FrameBuffer *otherFrameBuffer) {
    initializeNewFrameBuffer(otherFrameBuffer->_width, otherFrameBuffer->_height);
    copyFrameBuffer(otherFrameBuffer);

}

void FrameBuffer::copyFrameBuffer(FrameBuffer *otherFrameBuffer) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, otherFrameBuffer->_frameBuffer);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, otherFrameBuffer->_texture, 0);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    // Bind destination FBO + texture to another color attachment
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->_frameBuffer);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->_texture, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    // specify source, destination drawing (sub)rectangles.
    glBlitFramebuffer(0, 0, this->_width, this->_height,
                      0, 0, this->_width, this->_height,
                      GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT, GL_NEAREST);

    // unbind the color attachments
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);

    glBindTexture(GL_TEXTURE_2D,0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
}

FrameBuffer::~FrameBuffer() {
    destroy();
}

void FrameBuffer::destroy(){
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteTextures(1, &this->_texture);
    glDeleteRenderbuffers(1, &this->_depthRenderBuffer);
    glDeleteFramebuffers(1, &this->_frameBuffer);
}

void FrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    glViewport(0,0,_width,_height);
}

void FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bindTexture() {
    glBindTexture(GL_TEXTURE_2D, _texture);
}
