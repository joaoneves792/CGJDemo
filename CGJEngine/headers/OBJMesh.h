//
// Created by joao on 11/11/17.
//

#include "GL/glew.h"
#include <string>
#include <vector>

#ifndef CGJM_OBJMESH_H
#define CGJM_OBJMESH_H

#define VERTICES__ATTR 0
#define TEXCOORDS_ATTR 1
#define NORMALS__ATTR 2

#define VERTICES_VBO 0
#define TEXCOORDS_VBO 1
#define NORMALS_VBO 2

typedef struct {
    float vertex[3];
} objVertex;

typedef struct{
    float vertex[3];
    float normal[3];
    float texCoord[2];
    unsigned int index;
}objUniqueVertex;

typedef struct {
    float normal[3];
} objNormal;

typedef struct {
    float uv[2];
} objTexCoord;

typedef struct {
    unsigned int vertices[3];
    unsigned int normals[3];
    unsigned int texCoords[3];
} objFace;

typedef struct {
    unsigned int indexes[3];
}objUniqueFace;

typedef struct {
    std::vector<objVertex*> vertices;
    std::vector<objNormal*> normals;
    std::vector<objTexCoord*> texCoords;

    std::vector<objFace*> faces;

    GLuint vao;
    GLuint vbos[3];
    GLuint eab;

    unsigned long faceCount;

} objGroup;

class OBJMesh{
private:
   std::vector<objGroup*> groups;

public:
    OBJMesh();
    OBJMesh(const std::string& filename);
    void loadFromFile(const std::string& filename);
    void freeMeshData();
    void prepare();
    void unload();
    void draw();

};


#endif //CGJM_OBJMESH_H
