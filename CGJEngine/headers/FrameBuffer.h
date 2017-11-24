//
// Created by joao on 11/19/17.
//

#ifndef CGJDEMO_FRAMEBUFFER_H
#define CGJDEMO_FRAMEBUFFER_H

#include <GL/glew.h>

class FrameBuffer {
private:
    GLuint frameBuffer;
    GLuint depthRenderBuffer;
    GLuint texture;

    int width, height;
public:
    FrameBuffer(int x, int y);
    FrameBuffer(FrameBuffer* otherFrameBuffer);
    ~FrameBuffer();
    void bind();
    void unbind();
    void bindTexture();

private:
    void initializeNewFrameBuffer(int x, int y);
};


#endif //CGJDEMO_FRAMEBUFFER_H
