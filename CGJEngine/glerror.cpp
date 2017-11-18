#include <GL/glew.h>
#include <string>
#include <iostream>
#include "glerrors.h"

std::string parseGLError(int error){
    switch (error) {
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
        default:
            return "Unknown " + error;
    }
}

bool isOpenGLError() {
        bool isError = false;
        GLenum errCode;
        std::string errString;
        while ((errCode = glGetError()) != GL_NO_ERROR) {
                isError = true;
                errString = parseGLError(errCode);
                std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
        }
        return isError;
}

void checkOpenGLError(std::string error)
{
        if(isOpenGLError()) {
                std::cerr << error << std::endl;
                exit(EXIT_FAILURE);
        }
}

