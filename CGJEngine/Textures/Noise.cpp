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
#define inc(x) (++x%tile)
#define lerp(a, b, x) ( a + x * (b - a))
#define fade(t) (t * t * t * (t * (t * 6 - 15) + 10))

const int permutation[] = { 151,160,137,91,90,15,// Hash lookup table as defined by Ken Perlin.  This is a randomly
                            131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,// arranged array of all numbers from 0-255 inclusive.
                            190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
                            88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
                            77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
                            102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
                            135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
                            5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
                            223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
                            129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
                            251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
                            49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
                            138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};


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

float grad(int hash, float x, float y, float z) {
    int h = hash & 15;							// Take the hashed value and take the first 4 bits of it (15 == 0b1111)
    float u = h < 8 /* 0b1000 */ ? x : y;		// If the most significant bit (MSB) of the hash is 0 then set u = x.  Otherwise y.

    float v;								    // In Ken Perlin's original implementation this was another conditional operator (?:).  I
    // expanded it for readability.

    if(h < 4 /* 0b0100 */)						// If the first and second significant bits are 0 set v = y
        v = y;
    else if(h == 12 /* 0b1100 */ || h == 14 /* 0b1110*/)// If the first and second significant bits are 1 set v = x
        v = x;
    else 										// If the first and second significant bits are not equal (0/1, 1/0) set v = z
        v = z;

    return ((h&1) == 0 ? u : -u)+((h&2) == 0 ? v : -v); // Use the last 2 bits to decide if u and v are positive or negative.  Then return their addition.
}

float perlin(const int* p, int x, int y, int z, int tile, int size) {
    if(tile > 0) {									// If we have any repeat on, change the coordinates to their "local" repetitions
        x = x%tile;
        y = y%tile;
        z = z%tile;
    }

    int xi = x % 255;								// Calculate the "unit cube" that the point asked will be located in
    int yi = y % 255;								// The left bound is ( |_x_|,|_y_|,|_z_| ) and the right bound is that
    int zi = z % 255;								// plus 1.  Next we calculate the location (from 0.0 to 1.0) in that cube.
    float xf = (float)x/size;								// We also fade the location to smooth the result.
    float yf = (float)y/size;
    float zf = (float)z/size;
    float u = fade(xf);
    float v = fade(yf);
    float w = fade(zf);

    int aaa, aba, aab, abb, baa, bba, bab, bbb;
    aaa = p[p[p[    xi ]+    yi ]+    zi ];
    aba = p[p[p[    xi ]+inc(yi)]+    zi ];
    aab = p[p[p[    xi ]+    yi ]+inc(zi)];
    abb = p[p[p[    xi ]+inc(yi)]+inc(zi)];
    baa = p[p[p[inc(xi)]+    yi ]+    zi ];
    bba = p[p[p[inc(xi)]+inc(yi)]+    zi ];
    bab = p[p[p[inc(xi)]+    yi ]+inc(zi)];
    bbb = p[p[p[inc(xi)]+inc(yi)]+inc(zi)];

    float x1, x2, y1, y2;
    x1 = lerp(	grad (aaa, xf  , yf  , zf),				// The gradient function calculates the dot product between a pseudorandom
                  grad (baa, xf-1, yf  , zf),				// gradient vector and the vector from the input coordinate to the 8
                  u);										// surrounding points in its unit cube.
    x2 = lerp(	grad (aba, xf  , yf-1, zf),				// This is all then lerped together as a sort of weighted average based on the faded (u,v,w)
                  grad (bba, xf-1, yf-1, zf),				// values we made earlier.
                  u);
    y1 = lerp(x1, x2, v);

    x1 = lerp(	grad (aab, xf  , yf  , zf-1),
                  grad (bab, xf-1, yf  , zf-1),
                  u);
    x2 = lerp(	grad (abb, xf  , yf-1, zf-1),
                  grad (bbb, xf-1, yf-1, zf-1),
                  u);
    y2 = lerp (x1, x2, v);

    return (lerp (y1, y2, w)+1)/2;						// For convenience we bound it to 0 - 1 (theoretical min/max before is -1 - 1)
}

void Noise::generatePerlinNoise(int octaves, float persistence, float invisibleThreshold) {
    /* Perlin noise implementation adapted from sources at:
     * https://flafla2.github.io/2014/08/09/perlinnoise.html
     */
    auto p = new int[512];
    for(int x=0;x<512;x++) {
        p[x] = permutation[x%256];
    }
    float *data = createLayers();

    for(int layer=0; layer<_layers; layer++){
        for(int x=0; x<_size; x++){
            for(int y=0; y<_size; y++){
                float total = 0;
                int frequency = 1;
                float amplitude = 1;
                float maxValue = 0;			// Used for normalizing result to 0.0 - 1.0
                for(int i=0;i<octaves;i++) {
                    total += perlin(p, x * frequency, y * frequency, layer*frequency, 512, _size) * amplitude;

                    maxValue += amplitude;

                    amplitude *= persistence;
                    frequency *= 2;
                }

                float noise =  total/maxValue;
                DATA_R(layer, x, y) = lerp(_minRed, _maxRed, noise);
                DATA_G(layer, x, y) = lerp(_minGreen, _maxGreen, noise);
                DATA_B(layer, x, y) = lerp(_minBlue, _maxBlue, noise);
                if (noise < invisibleThreshold)
                    DATA_A(layer, x, y) = 0.0f;
                else
                    DATA_A(layer, x, y) = noise;
            }
        }
    }


    createTextures(data);
    delete[] data;
    delete[] p;
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