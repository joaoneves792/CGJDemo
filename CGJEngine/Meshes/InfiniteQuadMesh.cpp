//
// Created by joao on 11/11/17.
//

#include "Meshes/InfiniteQuadMesh.h"

InfiniteQuadMesh::InfiniteQuadMesh() {
    prepare();
}


void InfiniteQuadMesh::loadFromFile(const std::string& filename) {
    //empty
}

void InfiniteQuadMesh::freeMeshData() {
    //empty;
}

void InfiniteQuadMesh::prepare() {
    GLfloat vertices[] = {
             0.0f,  0.0f, 0.0f, 1.0f, //0
             1.0f,  0.0f, 0.0f, 0.0f, //1 x
             0.0f,  0.0f, 1.0f, 0.0f, //2 z
            -1.0f,  0.0f, 0.0f, 0.0f, //3 -x
             0.0f,  0.0f,-1.0f, 0.0f  //4 -z

    };
    GLushort indices[] = {
            0, 2, 1,
            0, 3, 2,
            0, 4, 3,
            0, 1, 4
    };

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    {
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(VERTICES__ATTR);
        glVertexAttribPointer(VERTICES__ATTR, 4, GL_FLOAT, GL_FALSE, sizeof(float)*4, 0);

        glGenBuffers(1, &_eab);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eab);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void InfiniteQuadMesh::unload() {
    glBindVertexArray(_vao);
    glDisableVertexAttribArray(VERTICES__ATTR);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_eab);
    glDeleteVertexArrays(1, &_vao);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void InfiniteQuadMesh::draw(){
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, (GLsizei)12, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}

void InfiniteQuadMesh::drawGroup(const std::string &name) {
    draw();
}