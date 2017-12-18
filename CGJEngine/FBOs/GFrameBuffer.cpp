//
// Created by joao on 12/5/17.
//

#include <cstdlib>
#include <iostream>
#include "FBOs/GFrameBuffer.h"

GFrameBuffer::GFrameBuffer(int x, int y) {
    _width = x;
    _height = y;
    _diffuseBuffer = new Texture();
    _ambientBuffer = new Texture();
    _normalBuffer = new Texture();
    _depthBuffer = new Texture();
    _specularBuffer = new Texture();
    initializeNewFrameBuffer(x, y);
}

void GFrameBuffer::initializeNewFrameBuffer(int x, int y) {
    _width = x;
    _height = y;

    glGenFramebuffers(1, &_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

    GLuint colorBuffer;
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);

    GLuint ambientBuffer;
    glGenTextures(1, &ambientBuffer);
    glBindTexture(GL_TEXTURE_2D, ambientBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, ambientBuffer, 0);

    GLuint specularBuffer;
    glGenTextures(1, &specularBuffer);
    glBindTexture(GL_TEXTURE_2D, specularBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, specularBuffer, 0);

    GLuint normalsBuffer;
    glGenTextures(1, &normalsBuffer);
    glBindTexture(GL_TEXTURE_2D, normalsBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, normalsBuffer, 0);

    GLuint depthBuffer;
    glGenTextures(1, &depthBuffer);
    glBindTexture(GL_TEXTURE_2D, depthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, _width, _height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);

    // Set the list of draw buffers.
    GLenum DrawBuffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(4, DrawBuffers); // "1" is the size of DrawBuffers

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Failed to create FrameBuffer" << std::endl;
        exit(EXIT_FAILURE);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    _diffuseBuffer->changeTexture(colorBuffer);
    _ambientBuffer->changeTexture(ambientBuffer);
    _specularBuffer->changeTexture(specularBuffer);
    _normalBuffer->changeTexture(normalsBuffer);
    _depthBuffer->changeTexture(depthBuffer);
}

void GFrameBuffer::resize(int x, int y) {
    destroy();
    _width = x;
    _height = y;
    initializeNewFrameBuffer(x, y);
}

void GFrameBuffer::destroy(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    _diffuseBuffer->destroyTexture();
    _ambientBuffer->destroyTexture();
    _specularBuffer->destroyTexture();
    _normalBuffer->destroyTexture();
    _depthBuffer->destroyTexture();
    glDeleteFramebuffers(1, &_frameBuffer);
}

void GFrameBuffer::bindDiffuse() {
    _diffuseBuffer->bind();
}

void GFrameBuffer::bindSpecular() {
    _specularBuffer->bind();
}

void GFrameBuffer::bindNormals() {
    _normalBuffer->bind();
}

void GFrameBuffer::bindDepth() {
    _depthBuffer->bind();
}

void GFrameBuffer::bindAmbient() {
    _ambientBuffer->bind();
}


void GFrameBuffer::blitDiffuse(ColorTextureFrameBuffer *destFBO) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destFBO->_frameBuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glBlitFramebuffer(0, 0, _width, _height,
                      0, 0, destFBO->_width, destFBO->_height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
}

void GFrameBuffer::blitAmbient(ColorTextureFrameBuffer *destFBO) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT1);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destFBO->_frameBuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glBlitFramebuffer(0, 0, _width, _height,
                      0, 0, destFBO->_width, destFBO->_height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);

}

void GFrameBuffer::blitSpecular(ColorTextureFrameBuffer *destFBO) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT2);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destFBO->_frameBuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glBlitFramebuffer(0, 0, _width, _height,
                      0, 0, destFBO->_width, destFBO->_height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);

}

void GFrameBuffer::blitNormal(ColorTextureFrameBuffer *destFBO) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT3);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destFBO->_frameBuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glBlitFramebuffer(0, 0, _width, _height,
                      0, 0, destFBO->_width, destFBO->_height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);

}

void GFrameBuffer::blitDepth(DepthTextureFrameBuffer *destFBO) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destFBO->_frameBuffer);

    glBlitFramebuffer(0, 0, _width, _height,
                      0, 0, destFBO->_width, destFBO->_height,
                      GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);

}

GFrameBuffer::~GFrameBuffer() {
    destroy();
    delete _diffuseBuffer;
    delete _specularBuffer;
    delete _normalBuffer;
    delete _depthBuffer;
    delete _ambientBuffer;
}

