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
#define TANGENTS__ATTR 3
#define BITANGENTS__ATTR 4
#define BONEINDICES__ATTR 5
#define BONEWEIGHTS__ATTR 6
#define PARTICLE_VERT_ATTR 7
#define PARTICLE_STATE_ATTR 8
#define SHAPEKEY_SLOT_1_ATTR 9
#define SHAPEKEY_SLOT_2_ATTR 10
#define SHAPEKEY_SLOT_3_ATTR 11

#define VERTICES_VBO 0
#define TEXCOORDS_VBO 1
#define NORMALS_VBO 2


#define TEXTURE_SLOT 0
#define ENVIRONMENT_SLOT 1

class Mesh{
public:
    virtual ~Mesh() = default;
    virtual void loadFromFile(const std::string& filename) = 0;
    virtual void freeMeshData() = 0;
    virtual void prepare() = 0;
    virtual void unload() = 0;
    virtual void draw() = 0;
    virtual void drawGroup(const std::string& name) = 0;
};


#endif //CGJM_MESH_H
