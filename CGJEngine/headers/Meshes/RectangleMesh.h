//
// Created by joao on 11/11/17.
//

#include "GL/glew.h"

#include "Meshes/Mesh.h"

#ifndef CGJM_RECTANGLEMESH_H
#define CGJM_RECTANGLEMESH_H


class RectangleMesh : public Mesh{
private:
    GLuint _vao;
    GLuint _vbo;
    GLuint _eab;

    float _width, _height, _length;

public:
    RectangleMesh(float width, float height, float length);
    void loadFromFile(const std::string& filename);
    void freeMeshData();
    void prepare();
    void unload();
    void draw();

};


#endif //CGJM_RECTANGLEMESH_H
