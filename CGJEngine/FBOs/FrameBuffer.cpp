//
// Created by joao on 11/19/17.
//
#include <GL/glew.h>
#include <cstdlib>
#include <iostream>

#include "FrameBuffer.h"

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

