//
// Created by joao on 11/11/17.
//

#include "Meshes/RectangleMesh.h"

RectangleMesh::RectangleMesh(float width, float height, float length) {
    _width = width;
    _height = height;
    _length = length;
    prepare();
}


void RectangleMesh::loadFromFile(const std::string &filename) {
    //empty
}

void RectangleMesh::freeMeshData() {
    //empty;
}

void RectangleMesh::prepare() {
#define W (_width)
#define H (_height)
#define L (_length)
#define W2 (_width/2.0f)
#define H2 (_height/2.0f)
#define L2 (_length/2.0f)
#define N (0.0f)

    GLfloat vertices[] = {
            //Bottom
            -W2, N, -L2, //0
             W2, N, -L2, //1
            -W2, N,  L2, //2
             W2, N,  L2, //3
            //Front
            -W2,  H, L2, //4
             W2,  H, L2, //5
            //Back
            -W2, H, -L2,//6
             W2, H, -L2,//7


    };
    GLushort indices[] ={
            //Bottom
            0, 1, 2,
            3, 2, 1,
            //Front
            4, 2, 3,
            5, 4, 3,
            //Back
            1, 0, 6,
            6, 7, 1,
            //Left
            6, 0, 2,
            2, 4, 6,
            //Right
            3, 1, 7,
            7, 5, 3,
            //Top
            7, 6, 4,
            4, 5, 7
    };

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    {
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(VERTICES__ATTR);
        glVertexAttribPointer(VERTICES__ATTR, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);

        glGenBuffers(1, &_eab);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eab);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void RectangleMesh::unload() {
    glBindVertexArray(_vao);
    glDisableVertexAttribArray(VERTICES__ATTR);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_eab);
    glDeleteVertexArrays(1, &_vao);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RectangleMesh::draw() {
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, (GLsizei)36, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}
