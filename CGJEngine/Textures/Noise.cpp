//
// Created by joao on 8/6/18.
//
#include <cstdlib>
#include <iostream>
#include <libnet.h>
#include <zmq.hpp>
#include "Textures/Noise.h"

const float INV_RAND_MAX = 1.0f / (RAND_MAX);
inline float rnd(float max=1.0f) { return max * INV_RAND_MAX * rand(); }
inline float rnd(float min, float max) { return min + (max - min) * INV_RAND_MAX * rand(); }
#define COLOR_BYTES 4
#define LAYER(layer) (_size*_size*(layer)*COLOR_BYTES)
#define Y(y) (_size*COLOR_BYTES*(y))
#define X(x) ((x)*COLOR_BYTES)
#define DATA_R(layer, x, y) data[LAYER(layer) + Y(y) + X(x) + 0]
#define DATA_G(layer, x, y) data[LAYER(layer) + Y(y) + X(x) + 1]
#define DATA_B(layer, x, y) data[LAYER(layer) + Y(y) + X(x) + 2]
#define DATA_A(layer, x, y) data[LAYER(layer) + Y(y) + X(x) + 3]


Noise::Noise(int layers, int size) {
    _layers = layers;
    _size = size;
    _textures = new GLuint[_layers];
    _minRed = 0.0f;
    _maxRed = 0.0f;
    _minGreen = 0.0f;
    _maxGreen = 0.0f;
    _minBlue = 0.0f;
    _maxBlue = 0.0f;


}

void Noise::setColor(float minRed, float minGreen, float minBlue, float maxRed, float maxGreen, float maxBlue) {
    _minRed = minRed;
    _minGreen = minGreen;
    _minBlue = minBlue;
    _maxRed = maxRed;
    _maxGreen = maxGreen;
    _maxBlue = maxBlue;
}

void Noise::createTextures(const float *data) const {
    GLuint texID = 0;
    for(int i=0; i < _layers; i++) {
        glGenTextures(1, &texID);   /* create the texture */
        glBindTexture(GL_TEXTURE_2D, texID);
        /* actually generate the texture */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _size, _size, 0, GL_RGBA, GL_FLOAT, &DATA_R(i, 0, 0));
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

}

float *Noise::createLayers() const {
    auto *data = new float[_layers * _size * _size * COLOR_BYTES];

    // Set all the pixels to the same colour, transparent black!
    for(int layer=0; layer < _layers; layer++)
        for (int x = 0; x < _size; x++)
            for (int y = 0; y < _size; y++) {
                DATA_R(layer, x, y) = 0.0f;
                DATA_G(layer, x, y) = 0.0f;
                DATA_B(layer, x, y) = 0.0f;
                DATA_A(layer, x, y) = 0.0f;
            }
    return data;
}

void Noise::generateSimpleNoise(int density, int seed){
    /*The following code is adapted from sources available at:
     * http://www.xbdev.net/directx3dx/specialX/Fur/
     */

    srand((unsigned int)seed);
    float *data = createLayers();


    for(int layer=0; layer < _layers; layer++) {
        float length = float(layer) / _layers; // 0 to 1
        length = 1 - length; // 1 to 0
        // *3 is just a value I picked by trial and error so the fur looks thick enough
        // doesn't really need to be here though!...can be adjusted externally
        int idensity = (int)(density * length* 3);

        // Alternatives for increasing density - creating different fur effects
        // Increasing by power
        // int density = idensity * pow(length,3);
        // Increasing sine
        //int density = idensity * sin(length*(D3DX_PI/2));
        srand(28382);
        for(int i=0; i<idensity; i++){
            int xrand = (int) rnd(0, _size);
            int yrand = (int) rnd(0, _size);
            DATA_R(layer, xrand, yrand) = rnd(_minRed, _maxRed);
            DATA_G(layer, xrand, yrand) = rnd(_minGreen, _maxGreen);
            DATA_B(layer, xrand, yrand) = rnd(_minBlue, _maxBlue);
            DATA_A(layer, xrand, yrand) = 1.0f;
        }
    }


    // Loop across all the pixels, and make the top layer of pixels more
// transparent (top hairs)
    for (int x = 0; x < _size; x++)
        for (int y = 0; y < _size; y++)
            for(int layer=0; layer < _layers; layer++){
                // length of the hair
                float length = (float)layer / _layers; // 0 to 1

                // tip of the hair is semi-transparent
                float alpha = DATA_A(layer, x, y);
                if( alpha > 0.0f )
                    DATA_A(layer, x, y) = 1.0f-length; // More transparent as we get closer to
                // the tip ,length is from 0 to 1, so the
                // tip or outer layer is 1.
            }

    // Well now, hairs that are closer to the center are darker as they get less light
    // so lets make hairs closer to the center darker?
        for (int x = 0; x < _size; x++)
            for (int y = 0; y < _size; y++)
                for(int layer=0; layer < _layers; layer++) {
                    // length of the hair
                    float length = (float)layer / _layers; // 0 to 1

                    // tip of the hair is semi-transparent
                    float scale = 1-length;
                    scale = (scale > 0.9f)?0.9f:scale;
                    float alpha = DATA_A(layer, x, y);
                    if( alpha > 0.0f ) {
                        DATA_R(layer, x, y) *= scale;
                        DATA_G(layer, x, y) *= scale;
                        DATA_B(layer, x, y) *= scale;
                    }
                }

    createTextures(data);
    delete[] data;
}

Noise::~Noise() {
    if(_textures) {
        glDeleteTextures(_layers, _textures);
        delete _textures;
    }
}

void Noise::bindLayerTexture(int layer) {
    glBindTexture(GL_TEXTURE_2D, _textures[layer]);
}