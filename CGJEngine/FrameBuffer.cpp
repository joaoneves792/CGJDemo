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
    this->width = x;
    this->height = y;

    glGenFramebuffers(1, &this->frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);
    glGenTextures(1, &this->texture);

    glBindTexture(GL_TEXTURE_2D, this->texture);

    // Give an empty image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glGenRenderbuffers(1, &this->depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, this->depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->width, this->height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depthRenderBuffer);


    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->texture, 0);

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

FrameBuffer::FrameBuffer(FrameBuffer *otherFrameBuffer) {
    initializeNewFrameBuffer(otherFrameBuffer->width, otherFrameBuffer->height);

    // Bind input FBO + texture to a color attachment
    glBindFramebuffer(GL_READ_FRAMEBUFFER, otherFrameBuffer->frameBuffer);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, otherFrameBuffer->texture, 0);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    // Bind destination FBO + texture to another color attachment
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, texture, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    // specify source, destination drawing (sub)rectangles.
    glBlitFramebuffer(0, 0, width, height,
                      0, 0, width, height,
                      GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT, GL_NEAREST);

    // unbind the color attachments
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);

    glBindTexture(GL_TEXTURE_2D,0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
}

FrameBuffer::~FrameBuffer() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &depthRenderBuffer);
    glDeleteFramebuffers(1, &frameBuffer);
}

void FrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(0,0,width,height);
}

void FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bindTexture() {
    glBindTexture(GL_TEXTURE_2D, texture);
}
