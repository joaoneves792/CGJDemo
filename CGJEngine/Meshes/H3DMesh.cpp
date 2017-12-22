#include <string.h>
#include <iostream>

#include "glm_wrapper.h"

#include <GL/glew.h>
#include <Meshes/H3DMesh.h>
#include "Meshes/H3DMesh.h"
#include "Texture.h"

H3DMesh::H3DMesh() {
    _vao = nullptr;
    _vbo = nullptr;
    _eab = nullptr;

    _groups = nullptr;
    _armatures = nullptr;
    _materials = nullptr;

    _groupCount = 0;
    _materialCount = 0;
    _armatureCount = 0;

    _currentFrame = 0;

    _isAnimated = false;

    _uploadMaterialCallback = nullptr;
}

H3DMesh::H3DMesh(const std::string &filename) {
    _vao = nullptr;
    _vbo = nullptr;
    _eab = nullptr;

    _groups = nullptr;
    _armatures = nullptr;
    _materials = nullptr;

    _groupCount = 0;
    _materialCount = 0;
    _armatureCount = 0;

    _currentFrame = 0;

    _isAnimated = false;

    _uploadMaterialCallback = nullptr;

    loadFromFile(filename);
    prepare();
    freeMeshData();
}


H3DMesh::~H3DMesh() {
    if(nullptr != _groups)
        unload();
}

void H3DMesh::Clear() {
    for(int i=0; i< _groupCount; i++){
        delete[] _groups[i].name;
    }

    freeMeshData();

    for(int i=0; i< _armatureCount; i++){
        delete[] _armatures[i].name;
    }

    delete[] _groups;
    _groups = nullptr;
    delete[] _armatures;
    _armatures = nullptr;

    _groupCount = 0;
    _materialCount = 0;
    _armatureCount = 0;
}

void H3DMesh::freeMeshData() {
    for(int i=0; i< _groupCount; i++){
        if(_groups[i].vertices != nullptr)
            delete[] _groups[i].vertices;
        if(_groups[i].triangles != nullptr)
            delete[] _groups[i].triangles;
        _groups[i].vertices = nullptr;
        _groups[i].triangles = nullptr;
    }
}

void H3DMesh::unload() {
    if(nullptr != _eab && nullptr != _vbo) {
        glDeleteBuffers(_groupCount, _eab);
        glDeleteBuffers(_groupCount, _vbo);
        _eab = nullptr;
        _vbo = nullptr;
        if(nullptr != _vao) {
            glDeleteVertexArrays(_groupCount, _vao);
            _vao = nullptr;
        }

    }
    for(int i=0; i< _materialCount; i++){
        glDeleteTextures(1, (GLuint *)&_materials[i].textureId);
    }

    Clear();
}

void H3DMesh::prepare() {
    _vbo = new GLuint [_groupCount];
    _eab = new GLuint [_groupCount];
    _vao = new GLuint [_groupCount];
    glGenVertexArrays(_groupCount, _vao);

    _vboDescriptions = new h3d_vboDescription[_groupCount];

    for(int i=0; i < _groupCount; i++){
            prepareGroup(&_groups[i], i);
    }
}


void H3DMesh::prepareGroup(h3d_group *group, unsigned int groupIndex) {
    glBindVertexArray(_vao[groupIndex]);

    auto *vertices  = new GLfloat[group->numVertices*3];
    auto *normals   = new GLfloat[group->numVertices*3];
    auto *texCoords = new GLfloat[group->numVertices*2];
    auto *bone_indices = new GLfloat[group->numVertices*BONE_COUNT];
    auto *bone_weights = new GLfloat[group->numVertices*BONE_COUNT];

    auto *indices  = new indexInt[group->numTriangles*3];

    int vi=0;
    int ni=0;
    int ti=0;
    int bi=0;

    for(int i=0;i<group->numVertices;i++){

        vertices[vi++] = group->vertices[i].vertex[0];
        vertices[vi++] = group->vertices[i].vertex[1];
        vertices[vi++] = group->vertices[i].vertex[2];

        normals[ni++] = group->vertices[i].normal[0];
        normals[ni++] = group->vertices[i].normal[1];
        normals[ni++] = group->vertices[i].normal[2];

        texCoords[ti++] = group->vertices[i].uv[0];
        texCoords[ti++] = group->vertices[i].uv[1];

        for(int k=0;k<BONE_COUNT;k++) {
            bone_indices[bi] = (GLfloat) group->vertices[i].boneID[k];
            bone_weights[bi++] = (GLfloat) group->vertices[i].weight[k];
        }
    }


    int ii=0;
    for(int i=0; i<group->numTriangles;i++){
        indices[ii++] = (indexInt)group->triangles[i].vertexIndices[0];
        indices[ii++] = (indexInt)group->triangles[i].vertexIndices[1];
        indices[ii++] = (indexInt)group->triangles[i].vertexIndices[2];
    }

    glGenBuffers(1, &_vbo[groupIndex]);

    _vboDescriptions[groupIndex].positionSize     = sizeof(GLfloat)*group->numVertices*3;
    _vboDescriptions[groupIndex].normalsSize      = sizeof(GLfloat)*group->numVertices*3;
    _vboDescriptions[groupIndex].textureCoordSize = sizeof(GLfloat)*group->numVertices*2;
    _vboDescriptions[groupIndex].jointsSize       = sizeof(GLfloat)*group->numVertices*BONE_COUNT;
    _vboDescriptions[groupIndex].weightsSize      = sizeof(GLfloat)*group->numVertices*BONE_COUNT;

    _vboDescriptions[groupIndex].totalSize = _vboDescriptions[groupIndex].positionSize
                                             + _vboDescriptions[groupIndex].normalsSize
                                             + _vboDescriptions[groupIndex].textureCoordSize
                                             + _vboDescriptions[groupIndex].jointsSize
                                             + _vboDescriptions[groupIndex].weightsSize;

    glBindBuffer(GL_ARRAY_BUFFER, _vbo[groupIndex]);
    glBufferData(GL_ARRAY_BUFFER, _vboDescriptions[groupIndex].totalSize, nullptr, GL_STATIC_DRAW);

    /*Copy the data to the buffer*/
    glBufferSubData(GL_ARRAY_BUFFER, 0, _vboDescriptions[groupIndex].positionSize, vertices);
    glBufferSubData(GL_ARRAY_BUFFER, _vboDescriptions[groupIndex].positionSize,
                    _vboDescriptions[groupIndex].textureCoordSize, texCoords);
    glBufferSubData(GL_ARRAY_BUFFER, _vboDescriptions[groupIndex].positionSize +
                                     _vboDescriptions[groupIndex].textureCoordSize,
                    _vboDescriptions[groupIndex].normalsSize, normals);
    glBufferSubData(GL_ARRAY_BUFFER, _vboDescriptions[groupIndex].positionSize +
                                     _vboDescriptions[groupIndex].textureCoordSize +
                                     _vboDescriptions[groupIndex].normalsSize,
                    _vboDescriptions[groupIndex].jointsSize, bone_indices);

    glBufferSubData(GL_ARRAY_BUFFER, _vboDescriptions[groupIndex].positionSize +
                                     _vboDescriptions[groupIndex].textureCoordSize +
                                     _vboDescriptions[groupIndex].normalsSize +
                                     _vboDescriptions[groupIndex].jointsSize,
                    _vboDescriptions[groupIndex].weightsSize, bone_weights);

    //Set up the indices
    glGenBuffers(1, &_eab[groupIndex]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eab[groupIndex]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexInt)*ii , indices, GL_STATIC_DRAW);


    //Position Attribute
    glVertexAttribPointer(VERTICES__ATTR, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, nullptr);
    glEnableVertexAttribArray(VERTICES__ATTR);

    //Texture coord attribute
    glVertexAttribPointer(TEXCOORDS__ATTR, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)_vboDescriptions[groupIndex].positionSize);
    glEnableVertexAttribArray(TEXCOORDS__ATTR);

    //Normals attribute
    glVertexAttribPointer(NORMALS__ATTR, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)
            (_vboDescriptions[groupIndex].positionSize+
             _vboDescriptions[groupIndex].textureCoordSize) );
    glEnableVertexAttribArray(NORMALS__ATTR);

    //bone indexes attribute
    glVertexAttribPointer(BONEINDICES__ATTR, BONE_COUNT, GL_FLOAT, GL_FALSE, 0, (GLvoid *)
            (_vboDescriptions[groupIndex].positionSize +
             _vboDescriptions[groupIndex].textureCoordSize +
             _vboDescriptions[groupIndex].normalsSize) );
    glEnableVertexAttribArray(BONEINDICES__ATTR);

    glVertexAttribPointer(BONEWEIGHTS__ATTR, BONE_COUNT, GL_FLOAT, GL_FALSE, 0, (GLvoid *)
            (_vboDescriptions[groupIndex].positionSize +
             _vboDescriptions[groupIndex].textureCoordSize +
             _vboDescriptions[groupIndex].normalsSize +
             _vboDescriptions[groupIndex].jointsSize) );
    glEnableVertexAttribArray(BONEWEIGHTS__ATTR);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //Free all the temporary memory
    delete[] vertices;
    delete[] indices;
    delete[] normals;
    delete[] texCoords;
    delete[] bone_indices;
    delete[] bone_weights;

}

void H3DMesh::setCurrentFrame(int f) {
    _currentFrame = f;
}

void H3DMesh::recursiveParentTransform(Mat4* transforms, bool* hasParentTransform, h3d_joint* joints, int jointIndex){
    int parentIndex =joints[jointIndex].parentIndex;
    if(parentIndex != -1 && !hasParentTransform[jointIndex]){
        recursiveParentTransform(transforms, hasParentTransform, joints, parentIndex);
        transforms[jointIndex] = transforms[parentIndex] * transforms[jointIndex];
        hasParentTransform[jointIndex] = true;
    }
}

Mat4 H3DMesh::getBoneTransform(h3d_joint* joint) {
    h3d_keyframe* prevKeyframe = nullptr;
    h3d_keyframe* nextKeyframe = nullptr;

    for(int j=0; j<joint->numKeyframes; j++){
        nextKeyframe = &joint->keyframes[j];
        if(j>0){
            prevKeyframe = &joint->keyframes[j-1];
        }else{
            prevKeyframe = nextKeyframe;
        }
        if(nextKeyframe->frame > _currentFrame){
            break;
        }
    }

    if(joint->numKeyframes > 0) {
        float lerpFactor;
        if (nextKeyframe->frame - prevKeyframe->frame == 0) {
            lerpFactor = 0;
        } else {
            lerpFactor = ((float)(_currentFrame - prevKeyframe->frame)) /
                         ((float)(nextKeyframe->frame - prevKeyframe->frame));
        }
        if (lerpFactor < 0)
            lerpFactor = 0;
        if (lerpFactor > 1)
            lerpFactor = 1;


        return glm::interpolate(prevKeyframe->transform, nextKeyframe->transform, lerpFactor);

    }
    return Mat4(1.0f);
}


Mat4 H3DMesh::getBindPose(h3d_joint* joint) {
    glm::mat4 bindPoseTranslation = glm::translate(glm::mat4(1.0f),
                                                   glm::vec3(joint->position[0],
                                                             joint->position[1],
                                                             joint->position[2]));

    glm::mat4 bindPoseRotation = glm::toMat4(glm::quat(glm::vec3(joint->rotation[0],
                                                                 joint->rotation[1],
                                                                 joint->rotation[2])));

    glm::mat4 bindPose = bindPoseTranslation * bindPoseRotation;
    return bindPose;
}

void H3DMesh::handleAnimation(h3d_group* group) {
    //Check for the Uniform, if its none existent then do nothing
    //GLint bones = glGetUniformLocation(_shader, "bones");
    //if(-1 == bones)
    //    return;

    h3d_armature* armature = &_armatures[group->armatureIndex];

    auto transforms = new Mat4[armature->jointsCount];
    for(int i=0; i<armature->jointsCount; i++){
        transforms[i] = Mat4(1.0f);
    }

    if(!group->isAnimated){ //If not animated the we still have to upload identities if the bones exist
        for(int i=0; i<armature->jointsCount; i++) {
            //glUniformMatrix4fv(bones + i, 1, GL_FALSE, glm::value_ptr(transforms[i]));
        }
        delete[] transforms;
        return;
    }


    for(int i=0; i<armature->jointsCount; i++){
        if(armature->joints[i].numKeyframes == 0)
            continue;

        glm::mat4 transform = getBoneTransform(&armature->joints[i]);

        glm::mat4 bindPose = armature->joints[i].bindPose;

        glm::mat4 invBindPose = armature->joints[i].invBindPose;

        transforms[i] = bindPose * transform * invBindPose;
    }

    //Recursively check and apply parent transforms
    auto hasParentTransform = new bool[armature->jointsCount];
    for(int i=0;i<armature->jointsCount;i++){
        hasParentTransform[i] = false;
    }

    for(int i=0; i<armature->jointsCount; i++) {
        recursiveParentTransform(transforms, hasParentTransform, armature->joints, i);
    }

    for(int i=0; i<armature->jointsCount; i++) {
        //glUniformMatrix4fv(bones + i, 1, GL_FALSE, glm::value_ptr(transforms[i]));
    }
    delete[] hasParentTransform;
    delete[] transforms;

}

void H3DMesh::draw() {
    for(int i=0; i < _groupCount; i++){
        //handleAnimation(&_groups[i]);
        int materialIndex = _groups[i].materialIndex;
        if( materialIndex >= 0 )
            setMaterial(&_materials[materialIndex]);
        else
            glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(_vao[i]);
        glDrawElements(GL_TRIANGLES, _groups[i].numTriangles*3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void H3DMesh::setMaterialUploadCallback(std::function<void(float ambient, float *diffuse, float *specular,
                                                           float *emissive, float shininess,
                                                           float transparency)> callback) {
    _uploadMaterialCallback = callback;
}

void H3DMesh::setMaterial(h3d_material *material){
    if(_uploadMaterialCallback != nullptr) {
        _uploadMaterialCallback(material->ambient, material->diffuse, material->specular,
                                material->emissive, material->shininess, material->transparency);
    }

    if( material->textureId >= 0){
        glActiveTexture(GL_TEXTURE0+TEXTURE_SLOT);
        glBindTexture( GL_TEXTURE_2D, (GLuint)material->textureId);
    }
}


void H3DMesh::loadFromFile(const std::string& filename) {
    FILE *fp = fopen(filename.c_str(), "rb");
    if (!fp)
        return;

    h3d_header header;
    fread(&header, 1, sizeof(h3d_header), fp);

    if (strncmp(header.id, "H3D", 3) != 0)
        return;

    if (header.version != 1)
        return;

    int numGroups;
    fread(&numGroups, 1, sizeof(int), fp);

    _groupCount = numGroups;
    _groups = new h3d_group[_groupCount];

    for(int i=0;i<numGroups;i++){

        //Get the name
        byte numChars;
        fread(&numChars, 1, sizeof(byte), fp);
        _groups[i].name = new char[numChars+1];
        fread(_groups[i].name, numChars, sizeof(char), fp);
        _groups[i].name[numChars] = '\0';

        //Get the material index
        fread(&_groups[i].materialIndex, 1, sizeof(int), fp);

        //Read the triangles and their vertex indexes
        fread(&_groups[i].numTriangles, 1, sizeof(int), fp);
        _groups[i].triangles = new h3d_triangle[_groups[i].numTriangles];
        for(int j=0; j<_groups[i].numTriangles;j++){
            fread(&_groups[i].triangles[j], 1, sizeof(h3d_triangle), fp);
        }

        //Read the vertex array (vertexes include their normal and UV)
        fread(&_groups[i].numVertices, 1, sizeof(int), fp);
        _groups[i].vertices = new h3d_vertex[_groups[i].numVertices];
        for(int j=0; j<_groups[i].numVertices; j++){
            /*fread(&_groups[i].vertices[j], 1, sizeof(h3d_vertex), fp);*/
            fread(_groups[i].vertices[j].vertex, 3, sizeof(float), fp);
            fread(_groups[i].vertices[j].normal, 3, sizeof(float), fp);
            fread(_groups[i].vertices[j].uv, 2, sizeof(float), fp);
            for(int k=0;k<BONE_COUNT;k++) {
                fread(&_groups[i].vertices[j].boneID[k], 1, sizeof(int), fp);
                fread(&_groups[i].vertices[j].weight[k], 1, sizeof(float), fp);
            }
        }

        fread(&_groups[i].isAnimated, 1, sizeof(byte), fp);
        if(_groups[i].isAnimated){
            byte numChars;
            fread(&numChars, 1, sizeof(byte), fp);
            _groups[i].armatureName = new char[numChars+1];
            fread(_groups[i].armatureName, numChars, sizeof(char), fp);
            _groups[i].armatureName[numChars] = '\0';
        }

        fread(&_groups[i].shapeKeyCount, 1, sizeof(int), fp);
        _groups[i].shapeKeys = new h3d_shape_key[_groups[i].shapeKeyCount];
        for(int j=0; j<_groups[i].shapeKeyCount; j++){
            byte numChars;
            fread(&numChars, 1, sizeof(byte), fp);
            _groups[i].shapeKeys[j].name = new char[numChars+1];
            fread(_groups[i].shapeKeys[j].name, numChars, sizeof(char), fp);
            _groups[i].shapeKeys[j].name[numChars] = '\0';


            fread(&_groups[i].shapeKeys[j].numVertices, 1, sizeof(int), fp);
            _groups[i].shapeKeys[j].vertices = new h3d_vertex[_groups[i].shapeKeys[j].numVertices];
            for(int k=0; k<_groups[i].shapeKeys[j].numVertices; k++) {
                fread(_groups[i].shapeKeys[j].vertices[k].vertex, 3, sizeof(float), fp);
                fread(_groups[i].shapeKeys[j].vertices[k].normal, 3, sizeof(float), fp);
            }
        }

    }

    fread(&_materialCount, 1, sizeof(int), fp);
    _materials = new h3d_material[_materialCount];

    std::string folderPath(filename);
    folderPath = folderPath.substr(0, folderPath.find_last_of("/")+1);

    for(int i=0;i<_materialCount;i++){
        //Get the texture file
        byte numChars;
        fread(&numChars, 1, sizeof(byte), fp);
        _materials[i].textureImage = new char[numChars+1];
        if(numChars > 0) {
            fread(_materials[i].textureImage, numChars, sizeof(char), fp);
            _materials[i].textureImage[numChars] = '\0';

            //Load the texture
            std::string texturePath("./");
            texturePath.assign(folderPath);
            texturePath.append(_materials[i].textureImage);
            _materials[i].textureId = Texture::LoadGLTexture(texturePath.c_str());
        }else {
            _materials[i].textureId = -1;
        }

        fread(&_materials[i].ambient, 1, sizeof(float), fp);
        fread(&_materials[i].diffuse, 3, sizeof(float), fp);
        fread(&_materials[i].specular, 3, sizeof(float), fp);
        fread(&_materials[i].emissive, 3, sizeof(float), fp);
        fread(&_materials[i].shininess, 1, sizeof(float), fp);
        fread(&_materials[i].transparency, 1, sizeof(float), fp);
        _materials[i].diffuse[3] = 1.0f;
        _materials[i].specular[3] = 1.0f;
        _materials[i].emissive[3] = 1.0f;
    }

    fread(&_armatureCount, 1, sizeof(int), fp);
    _armatures = new h3d_armature[_armatureCount];
    for(int i=0;i<_armatureCount;i++){
        //Get the Armature name file
        byte numChars;
        fread(&numChars, 1, sizeof(byte), fp);
        _armatures[i].name = new char[numChars+1];
        fread(_armatures[i].name, numChars, sizeof(char), fp);
        _armatures[i].name[numChars] = '\0';

        fread(&_armatures[i].jointsCount, 1, sizeof(int), fp);
        _armatures[i].joints = new h3d_joint[_armatures[i].jointsCount];

        for(int j=0;j<_armatures[i].jointsCount;j++){
            fread(&numChars, 1, sizeof(byte), fp);
            _armatures[i].joints[j].name = new char[numChars+1];
            fread(_armatures[i].joints[j].name, numChars, sizeof(char), fp);
            _armatures[i].joints[j].name[numChars] = '\0';

            fread(&_armatures[i].joints[j].position[0], 1, sizeof(float), fp);
            fread(&_armatures[i].joints[j].position[1], 1, sizeof(float), fp);
            fread(&_armatures[i].joints[j].position[2], 1, sizeof(float), fp);

            fread(&_armatures[i].joints[j].rotation[0], 1, sizeof(float), fp);
            fread(&_armatures[i].joints[j].rotation[1], 1, sizeof(float), fp);
            fread(&_armatures[i].joints[j].rotation[2], 1, sizeof(float), fp);

            fread(&_armatures[i].joints[j].parentIndex, 1, sizeof(int), fp);

            //Calculate the bindpose matrix
            _armatures[i].joints[j].bindPose = getBindPose(&_armatures[i].joints[j]);
            _armatures[i].joints[j].invBindPose = glm::inverse(_armatures[i].joints[j].bindPose);

            fread(&_armatures[i].joints[j].numKeyframes, 1, sizeof(int), fp);
            _armatures[i].joints[j].keyframes = new h3d_keyframe[_armatures[i].joints[j].numKeyframes];
            for(int k=0;k<_armatures[i].joints[j].numKeyframes;k++){
                fread(&_armatures[i].joints[j].keyframes[k].frame, 1, sizeof(int), fp);
                fread(_armatures[i].joints[j].keyframes[k].position, 3, sizeof(float), fp);
                fread(_armatures[i].joints[j].keyframes[k].rotation, 3, sizeof(float), fp);
                float x, y, z;
                h3d_keyframe* keyframe = &_armatures[i].joints[j].keyframes[k];

                x = keyframe->rotation[0];
                y = keyframe->rotation[1];
                z = keyframe->rotation[2];
                Vec3 euler = Vec3(x, y, z);

                x = keyframe->position[0];
                y = keyframe->position[1];
                z = keyframe->position[2];
                keyframe->transform = glm::translate(Mat4(1.0f), Vec3(x, y, z)) * glm::toMat4(glm::quat(euler));
            }

        }

    }

    for(int i=0;i<_groupCount;i++){
        if(_groups[i].isAnimated){
            for(int j=0;j<_armatureCount;j++){
                if(!strcmp(_groups[i].armatureName, _armatures[j].name)){
                    _groups[i].armatureIndex = j;
                }
            }
        }
    }
}