//
// Created by joao on 11/11/17.
//

#include "Meshes/QuadMesh.h"

QuadMesh::QuadMesh() {
    prepare();
}


void QuadMesh::loadFromFile(const std::string& filename) {
    //empty
}

void QuadMesh::freeMeshData() {
    //empty;
}

void QuadMesh::prepare() {
    GLfloat vertices[] = {
            -1.0f, -1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,
             1.0f, -1.0f, 0.0f

    };
    GLushort indices[] = {
            0, 1, 2,
            0, 3, 1
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

void QuadMesh::unload() {
    glBindVertexArray(_vao);
    glDisableVertexAttribArray(VERTICES__ATTR);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_eab);
    glDeleteVertexArrays(1, &_vao);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void QuadMesh::draw(){
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, (GLsizei)6, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}

void QuadMesh::drawGroup(const std::string &name) {
    draw();
}
