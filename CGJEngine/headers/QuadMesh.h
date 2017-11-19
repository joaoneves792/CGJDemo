//
// Created by joao on 11/11/17.
//

#include "GL/glew.h"

#include "Mesh.h"

#ifndef CGJM_QUADMESH_H
#define CGJM_QUADMESH_H


class QuadMesh : public Mesh{
private:
    GLuint vao;
    GLuint vbo;
    GLuint eab;

public:
    QuadMesh();
    void loadFromFile(const std::string& filename);
    void freeMeshData();
    void prepare();
    void unload();
    void draw();

};


#endif //CGJM_QUADMESH_H
