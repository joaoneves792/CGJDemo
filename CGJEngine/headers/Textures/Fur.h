//
// Created by joao on 8/6/18.
//

#ifndef CGJDEMO_FUR_H
#define CGJDEMO_FUR_H

#include <GL/glew.h>

class Fur{
private:
    int _layers;
    int _size;
    GLuint* _textures;
public:
    Fur(int layers, int size, int density, int seed, float red, float green, float blue);
    void bindLayerTexture(int layer);
    ~Fur();
};


#endif //CGJDEMO_FUR_H
