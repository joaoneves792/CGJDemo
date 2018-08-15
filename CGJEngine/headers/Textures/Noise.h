//
// Created by joao on 8/6/18.
//

#ifndef CGJDEMO_FUR_H
#define CGJDEMO_FUR_H

#include <GL/glew.h>

class Noise{
private:
    int _layers;
    int _size;
    GLuint* _textures;
    float _minRed;
    float _minGreen;
    float _minBlue;
    float _maxRed;
    float _maxGreen;
    float _maxBlue;
    int _mipmapLevel;
public:
    Noise(int layers, int size);
    void setColor(float minRed, float minGreen, float minBlue, float maxRed, float maxGreen, float maxBlue);
    void setMaxMipmapLevel(int level);
    void generateSimpleNoise(int density, int seed);
    void generatePerlinNoise(int octaves, float persistence, float invisibleThreshold);
    void bindLayerTexture(int layer);
    ~Noise();

private:
    float *createLayers() const;
    void createTextures(float *data) const;
};


#endif //CGJDEMO_FUR_H
