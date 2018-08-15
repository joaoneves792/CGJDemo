//
// Created by joao on 11/11/17.
//

#include "Meshes/OBJMesh.h"
#include <iomanip> // setprecision
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <Meshes/OBJMesh.h>
#include <unordered_map>
#include <list>

OBJMesh::OBJMesh() =default;

OBJMesh::OBJMesh(const std::string &filename) {
    loadFromFile(filename);
    prepare();
    freeMeshData();
}

void parseVertex(std::stringstream& sin, objGroup* group)
{
    auto v = new objVertex;
    sin >> v->vertex[0] >> v->vertex[1] >> v->vertex[2];
    group->vertices.push_back(v);
}

void parseTexcoord(std::stringstream& sin, objGroup* group)
{
    auto t = new objTexCoord;
    sin >> t->uv[0] >> t->uv[1];
    group->texCoords.push_back(t);
}

void parseNormal(std::stringstream& sin, objGroup* group)
{
    auto n = new objNormal;
    sin >> n->normal[0] >> n->normal[1] >> n->normal[2];
    group->normals.push_back(n);
}

void parseFace(std::stringstream& sin, objGroup* group)
{
    std::string token;
    auto f = new objFace;
    group->faces.push_back(f);
    for(int i = 0; i < 3; i++) {
        std::getline(sin, token, '/');
        if(!token.empty()) f->vertices[i] = (unsigned int)std::stoi(token);
        std::getline(sin, token, '/');
        if(!token.empty()) f->texCoords[i] = (unsigned int)std::stoi(token);
        std::getline(sin, token, ' ');
        if(!token.empty()) f->normals[i] = (unsigned int)std::stoi(token);

    }
}
void parseLine(std::stringstream& sin, objGroup* group){
    std::string s;
    sin >> s;
    if(s.compare("v" ) == 0) parseVertex(sin, group);
    else if(s.compare("vt") == 0) parseTexcoord(sin, group);
    else if(s.compare("vn") == 0) parseNormal(sin, group);
    else if(s.compare("f" ) == 0) parseFace(sin, group);
}

void OBJMesh::loadFromFile(const std::string& filename) {
    std::ifstream ifile(filename);
    std::string line;

    auto group = new objGroup;
    _groups.push_back(group);

    while(std::getline(ifile, line)) {
        auto sin = std::stringstream(line);
        parseLine(sin, group);
    }
    group->faceCount = (unsigned long)group->faces.size();
}

void OBJMesh::freeMeshData() {
    for(objGroup* g : _groups){
        for(objVertex* v : g->vertices)
            delete v;
        g->vertices.clear();

        for(objNormal* n : g->normals)
            delete n;
        g->normals.clear();

        for(objTexCoord* t : g->texCoords)
            delete t;
        g->texCoords.clear();

        for(objFace* f : g->faces)
            delete f;
        g->faces.clear();

    }
}


void prepareGroup(objGroup* group){
    const int PRECISION = 5;
    glGenVertexArrays(1, &(group->vao));
    glBindVertexArray(group->vao);
    {

        /*Identify unique vertexes (same pos/nomals/texcoords) and get them on a vector*/

        /*First put them in a hashtable to check for uniqueness*/
        unsigned int currentIndex = 0;

        auto uniqueFaces = new objUniqueFace[group->faces.size()];
        unsigned int faceIndex = 0;

        std::unordered_map<std::string, objUniqueVertex*> map;
        for(objFace* f: group->faces){
            for(int i=0; i<3; i++){
                objVertex* v = group->vertices[f->vertices[i]-1];
                objNormal* n;
                objTexCoord* t;
                std::stringstream stream;
                stream << std::fixed << std::setprecision(PRECISION) <<
                       v->vertex[0] << ":" << v->vertex[1] << ":" << v->vertex[2];
                if(!group->normals.empty()) {
                    n = group->normals[f->normals[i]-1];
                    stream << std::fixed << std::setprecision(PRECISION) << "/" <<
                           n->normal[0] << ":" << n->normal[1] << ":" << n->normal[2];
                }
                if(!group->texCoords.empty()){
                    t = group->texCoords[f->texCoords[i]-1];
                    stream << std::fixed << std::setprecision(PRECISION) << "/" <<
                           t->uv[0] << ":" << t->uv[1];
                }
                std::string s = stream.str();

                auto search = map.find(s);
                if(search != map.end()) {
                    //Found an exact same vertex place its index in the face
                    uniqueFaces[faceIndex].indexes[i] = search->second->index;
                }else{
                    //No matching vertex found create it and assign it to the unique face
                    auto uniqueVertex = new objUniqueVertex;
                    uniqueVertex->vertex[0] = v->vertex[0];
                    uniqueVertex->vertex[1] = v->vertex[1];
                    uniqueVertex->vertex[2] = v->vertex[2];
                    if(!group->normals.empty()) {
                        uniqueVertex->normal[0] = n->normal[0];
                        uniqueVertex->normal[1] = n->normal[1];
                        uniqueVertex->normal[2] = n->normal[2];
                    }
                    if(!group->texCoords.empty()){
                        uniqueVertex->texCoord[0] = t->uv[0];
                        uniqueVertex->texCoord[1] = t->uv[1];
                    }
                    uniqueVertex->index = currentIndex;
                    uniqueFaces[faceIndex].indexes[i] = currentIndex;
                    currentIndex++;
                    map[s] = uniqueVertex;
                }
            }
            faceIndex++;
        }

        /*Now put them in a list and sort them*/
        std::list<objUniqueVertex*> uniqueVertexList;
        for(auto it = map.begin(); it!= map.end(); it++){
            uniqueVertexList.push_back(it->second);
        }
        map.clear();

        uniqueVertexList.sort([](const objUniqueVertex* a, const objUniqueVertex* b){ return (a->index < b->index);});
        std::vector<objUniqueVertex*> uniqueVertexVector;
        for(objUniqueVertex* v : uniqueVertexList){
            uniqueVertexVector.push_back(v);
        }
        uniqueVertexList.clear();

        auto vertices  = new GLfloat[uniqueVertexVector.size()*3];
        GLfloat *normals;
        GLfloat *texCoords;

        if(!group->normals.empty()) {
            normals = new GLfloat[uniqueVertexVector.size() * 3];
        }
        if(!group->texCoords.empty()) {
            texCoords = new GLfloat[uniqueVertexVector.size() * 2];
        }
        auto indices  = new GLushort[group->faces.size()*3];

        GLushort vi=0;
        GLushort ni=0;
        GLushort ti=0;
        GLushort ii=0;


        for(objUniqueVertex* v: uniqueVertexVector){
            vertices[vi++] = v->vertex[0];
            vertices[vi++] = v->vertex[1];
            vertices[vi++] = v->vertex[2];

            if(!group->normals.empty()) {
                normals[ni++] = v->normal[0];
                normals[ni++] = v->normal[1];
                normals[ni++] = v->normal[2];
            }
            if(!group->texCoords.empty()){
                texCoords[ti++] = v->texCoord[0];
                texCoords[ti++] = v->texCoord[1];
            }
        }

        for(size_t i=0; i< group->faces.size();i++){
            objUniqueFace f = uniqueFaces[i];
            indices[ii++] = (GLshort )f.indexes[0];
            indices[ii++] = (GLshort )f.indexes[1];
            indices[ii++] = (GLshort )f.indexes[2];
        }

        glGenBuffers(3, group->vbos);
        glBindBuffer(GL_ARRAY_BUFFER, group->vbos[VERTICES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, vi * sizeof(float), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(VERTICES__ATTR);
        glVertexAttribPointer(VERTICES__ATTR, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);

        if (ti > 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, group->vbos[TEXCOORDS_VBO]);
            glBufferData(GL_ARRAY_BUFFER, ti * sizeof(float), texCoords, GL_STATIC_DRAW);
            glEnableVertexAttribArray(TEXCOORDS__ATTR);
            glVertexAttribPointer(TEXCOORDS__ATTR, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);
        }
        if (ni > 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, group->vbos[NORMALS_VBO]);
            glBufferData(GL_ARRAY_BUFFER, ni * sizeof(float), normals, GL_STATIC_DRAW);
            glEnableVertexAttribArray(NORMALS__ATTR);
            glVertexAttribPointer(NORMALS__ATTR, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);
        }
        glGenBuffers(1, &(group->eab));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, group->eab);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*ii , indices, GL_STATIC_DRAW);

        //Free all the temporary memory
        delete[] uniqueFaces;
        delete[] vertices;
        delete[] indices;
        if(!group->normals.empty()) {
            delete[] normals;
        }
        if(!group->texCoords.empty()) {
            delete[] texCoords;
        }

        for(objUniqueVertex* v: uniqueVertexVector){
            delete v;
        }
        uniqueVertexVector.clear();
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OBJMesh::prepare() {
    for(objGroup* group : _groups) {
        prepareGroup(group);
    }
}

void unloadGroup(objGroup* group){
    glBindVertexArray(group->vao);
    glDisableVertexAttribArray(VERTICES__ATTR);
    glDisableVertexAttribArray(TEXCOORDS__ATTR);
    glDisableVertexAttribArray(NORMALS__ATTR);
    glDeleteBuffers(3, group->vbos);
    glDeleteBuffers(1, &group->eab);
    glDeleteVertexArrays(1, &group->vao);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void OBJMesh::unload() {
    for(objGroup* group : _groups)
        unloadGroup(group);
}

void _drawGroup(objGroup* group){
    glBindVertexArray(group->vao);
    glDrawElements(GL_TRIANGLES, (GLsizei)group->faceCount*3, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}

void OBJMesh::draw(){
    for(objGroup* group : _groups)
        _drawGroup(group);
}
void OBJMesh::drawGroup(const std::string &name) {
    std::cout << "Abort: method drawGroup of OBJMesh not implemented!" << std::endl;
    exit(-1);
}
