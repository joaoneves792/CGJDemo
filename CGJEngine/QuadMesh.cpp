//
// Created by joao on 11/11/17.
//

#include "QuadMesh.h"

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

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(VERTICES__ATTR);
        glVertexAttribPointer(VERTICES__ATTR, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);

        glGenBuffers(1, &eab);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void QuadMesh::unload() {
    glBindVertexArray(vao);
    glDisableVertexAttribArray(VERTICES__ATTR);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &eab);
    glDeleteVertexArrays(1, &vao);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void QuadMesh::draw(){
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (GLsizei)6, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}
