//
// Created by joao on 11/11/17.
//

#include "GL/glew.h"
#include <string>
#include <vector>

#ifndef CGJM_MESH_H
#define CGJM_MESH_H

#define VERTICES__ATTR 0
#define TEXCOORDS__ATTR 1
#define NORMALS__ATTR 2
#define BONEINDICES__ATTR 3
#define BONEWEIGHTS__ATTR 4
#define PARTICLE_VERT_ATTR 5
#define PARTICLE_POS_ATTR 6
#define PARTICLE_LIFE_ATTR 7

#define VERTICES_VBO 0
#define TEXCOORDS_VBO 1
#define NORMALS_VBO 2

class Mesh{
public:
    virtual ~Mesh() = default;
    virtual void loadFromFile(const std::string& filename) = 0;
    virtual void freeMeshData() = 0;
    virtual void prepare() = 0;
    virtual void unload() = 0;
    virtual void draw() = 0;
};


#endif //CGJM_MESH_H
