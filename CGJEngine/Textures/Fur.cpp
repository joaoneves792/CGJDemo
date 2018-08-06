//
// Created by joao on 8/6/18.
//
#include <cstdlib>
#include <iostream>
#include "Textures/Fur.h"

const float INV_RAND_MAX = 1.0f / (RAND_MAX);
inline float rnd(float max=1.0f) { return max * INV_RAND_MAX * rand(); }
inline float rnd(float min, float max) { return min + (max - min) * INV_RAND_MAX * rand(); }


Fur::Fur(int layers, int size, int density, int seed, float red, float green, float blue) {
    _layers = layers;
    _size = size;
    _textures = new GLuint[layers];

    /*The following code is adapted from sources available at:
     * http://www.xbdev.net/directx3dx/specialX/Fur/
     */


    srand((unsigned int)seed);

    #define COLOR_BYTES 4
    #define LAYER(layer) (size*size*(layer)*COLOR_BYTES)
    #define Y(y) (size*COLOR_BYTES*(y))
    #define X(x) ((x)*COLOR_BYTES)
    auto *data = new float[layers*size*size*COLOR_BYTES];
    #define DATA_R(layer, x, y) data[LAYER(layer) + Y(y) + X(x) + 0]
    #define DATA_G(layer, x, y) data[LAYER(layer) + Y(y) + X(x) + 1]
    #define DATA_B(layer, x, y) data[LAYER(layer) + Y(y) + X(x) + 2]
    #define DATA_A(layer, x, y) data[LAYER(layer) + Y(y) + X(x) + 3]

    // Set all the pixels to the same colour, transparent black!
    for(int layer=0; layer<layers; layer++)
        for (int x = 0; x < size; x++)
            for (int y = 0; y < size; y++) {
                DATA_R(layer, x, y) = 0.0f;
                DATA_G(layer, x, y) = 0.0f;
                DATA_B(layer, x, y) = 0.0f;
                DATA_A(layer, x, y) = 0.0f;
            }


    for(int layer=0; layer<layers; layer++) {
        float length = float(layer)/layers; // 0 to 1
        length = 1 - length; // 1 to 0
        // *3 is just a value I picked by trial and error so the fur looks thick enough
        // doesn't really need to be here though!...can be adjusted externally
        int idensity = (int)density * length* 3;

        // Alternatives for increasing density - creating different fur effects
        // Increasing by power
        // int density = idensity * pow(length,3);
        // Increasing sine
        //int density = idensity * sin(length*(D3DX_PI/2));
        srand(28382);
        for(int i=0; i<idensity; i++){
            int xrand = (int)rnd(0, size);
            int yrand = (int)rnd(0, size);
            DATA_R(layer, xrand, yrand) = red;
            DATA_G(layer, xrand, yrand) = green;
            DATA_B(layer, xrand, yrand) = blue;
            DATA_A(layer, xrand, yrand) = 1.0f;
        }
    }


    // Loop across all the pixels, and make the top layer of pixels more
    // transparent (top hairs)
    for (int x = 0; x < size; x++)
        for (int y = 0; y < size; y++)
            for(int layer=0; layer<layers; layer++){
                // length of the hair
                float length = (float)layer/layers; // 0 to 1

                // tip of the hair is semi-transparent
                float alpha = DATA_A(layer, x, y);
                if( alpha > 0.0f )
                    DATA_A(layer, x, y) = 1.0f-length; // More transparent as we get closer to
                // the tip ,length is from 0 to 1, so the
                // tip or outer layer is 1.
            }

    // Well now, hairs that are closer to the center are darker as they get less light
    // so lets make hairs closer to the center darker?
    {
        for (int x = 0; x < size; x++)
            for (int y = 0; y < size; y++)
                for(int layer=0; layer<layers; layer++)
                {
                    // length of the hair
                    float length = (float)layer/layers; // 0 to 1

                    // tip of the hair is semi-transparent
                    float scale = 1-length;
                    scale = (scale > 0.9f)?0.9f:scale;
                    float alpha = DATA_A(layer, x, y);
                    if( alpha > 0.0f )
                    {
                        DATA_R(layer, x, y) *= scale;
                        DATA_G(layer, x, y) *= scale;
                        DATA_B(layer, x, y) *= scale;
                    }
                }
    }

    GLuint texID = 0;
    for(int i=0; i<layers; i++) {
        glGenTextures(1, &texID);   /* create the texture */
        glBindTexture(GL_TEXTURE_2D, texID);
        /* actually generate the texture */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_FLOAT, &DATA_R(i, 0, 0));
        if(GLEW_EXT_texture_filter_anisotropic){
            GLfloat largestAnisotropic;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largestAnisotropic);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largestAnisotropic);
        }else{
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        _textures[i] = texID;
    }

    delete[] data;

}

Fur::~Fur() {
    if(_textures) {
        glDeleteTextures(_layers, _textures);
        delete _textures;
    }
}

void Fur::bindLayerTexture(int layer) {
    glBindTexture(GL_TEXTURE_2D, _textures[layer]);
}