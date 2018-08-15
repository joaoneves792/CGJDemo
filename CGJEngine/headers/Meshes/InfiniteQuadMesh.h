//
// Created by joao on 11/11/17.
//

#include "GL/glew.h"

#include "Meshes/Mesh.h"

#ifndef CGJM_INFINITEQUADMESH_H
#define CGJM_INFINITEQUADMESH_H


class InfiniteQuadMesh : public Mesh{
private:
    GLuint _vao;
    GLuint _vbo;
    GLuint _eab;

public:
    InfiniteQuadMesh();
    void loadFromFile(const std::string& filename);
    void freeMeshData();
    void prepare();
    void unload();
    void draw();
    void drawGroup(const std::string& name);

};


#endif //CGJM_INFINITEQUADMESH_H
