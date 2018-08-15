//
// Created by joao on 11/11/17.
//

#include "GL/glew.h"

#include "Meshes/Mesh.h"

#ifndef CGJM_QUADMESH_H
#define CGJM_QUADMESH_H


class QuadMesh : public Mesh{
private:
    GLuint _vao;
    GLuint _vbo;
    GLuint _eab;

public:
    QuadMesh();
    void loadFromFile(const std::string& filename);
    void freeMeshData();
    void prepare();
    void unload();
    void draw();
    void drawGroup(const std::string& name);

};


#endif //CGJM_QUADMESH_H
