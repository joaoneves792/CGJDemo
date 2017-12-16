/*
 *	LXSR Load textures
 * 	adapted from multiple sources by E 
 *	Texture.h
 *
 *  Created on: Jun 2, 2012
 */
#ifndef TEXTURES_H_
#define TEXTURES_H_

#include <GL/glew.h>

typedef struct {
    int width;
    int height;
    bool alpha;
    bool compressed;
    GLsizei data_lenght;
    unsigned char *data;
}textureImage;

class Texture{
private:
    GLuint _texture;
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

    static GLuint LoadGLTexture(const char *filename);
private:
    static textureImage* LoadFromFile(const char* name);

};
#endif /* TEXTURES_H_*/
