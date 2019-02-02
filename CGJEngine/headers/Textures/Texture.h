/*
 *	LXSR Load textures
 * 	adapted from multiple sources by E 
 *	Texture.h
 *
 *  Created on: Jun 2, 2012
 */
#ifndef TEXTURES_H_
#define TEXTURES_H_

#include <string>
#include <GL/glew.h>

extern "C" {
#ifdef FREEIMG
#include <FreeImage.h>
#endif
}

typedef struct {
#ifdef FREEIMG
    FIBITMAP* fibitmap;
#endif
    int width;
    int height;
    GLint internalFormat;
    GLenum format;
    GLenum type;
    bool compressed;
    GLsizei data_lenght;
    unsigned char *data;
    GLuint GLtexture;
}textureImage;

class Texture{
private:
    GLuint _texture;
    int _width;
    int _height;
    const std::string _name;
public:
    Texture(std::string filename);
    Texture(const std::string& right, const std::string& left, const std::string& top,
            const std::string& bottom, const std::string& back, const std::string& front);
    Texture();
    Texture(GLuint texture);

    GLuint getTexture();
    void changeTexture(GLuint texture);
    void destroyTexture();
    void bind();
    void bindCubeMap();
    void generateRandom(int width);
    const std::string& getName();
    int getWidth();
    int getHeight();

    static textureImage* LoadGLTexture(const char *filename);
private:
    static textureImage* LoadFromFile(const char* name);

};
#endif /* TEXTURES_H_*/
