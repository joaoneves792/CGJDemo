#include <string.h>
#include <iostream>
#include <string.h>

#include "glm_wrapper.h"

#include <GL/glew.h>
#include <Meshes/H3DMesh.h>
#include <CGJengine.h>
#include "Meshes/H3DMesh.h"
#include "Textures/Texture.h"

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

    _isAnimated = false;
    _currentFrame = 0;

    _uploadMaterialCallback = nullptr;
    _boneUploadCallback = nullptr;
    _shapeKeyPercentCallback = nullptr;
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

    _isAnimated = false;

    _uploadMaterialCallback = nullptr;
    _boneUploadCallback = nullptr;
    _shapeKeyPercentCallback = nullptr;
    _currentFrame = 0;

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
        for(int j=0; j<_groups[i].shapeKeyCount; j++){
            auto sk = _groups[i].shapeKeys[j];
            delete[] sk.name;
        }
        _groups[i].shapeKeyCount = 0;
        delete[] _groups[i].shapeKeys;
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
        for(int j=0; j<_groups[i].shapeKeyCount; j++){
            auto sk = _groups[i].shapeKeys[j];
            delete[] sk.vertices;
        }
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
        for(unsigned int j=0; j<_materials[i].textureCount; j++) {
            std::string name = std::string(_materials[i].texture[j]->getName());
            std::cout << "Freeing " << name << std::endl;
            _materials[i].texture[j].reset();
            ResourceManager::getInstance()->destroyTexture(name);
        }
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
    auto *tangents  = new GLfloat[group->numVertices*3];
    auto *bitangents  = new GLfloat[group->numVertices*3];
    auto *texCoords = new GLfloat[group->numVertices*2];
    auto *bone_indices = new GLfloat[group->numVertices*BONE_COUNT];
    auto *bone_weights = new GLfloat[group->numVertices*BONE_COUNT];
    auto *shape_keys = new GLfloat[group->numVertices*3*group->shapeKeyCount];

    auto *indices  = new indexInt[group->numTriangles*3];

    int vi=0;
    int ni=0;
    int ti=0;
    int bi=0;

    for(int i=0;i<group->numVertices;i++){

        vertices[vi+0] = group->vertices[i].vertex[0];
        vertices[vi+1] = group->vertices[i].vertex[1];
        vertices[vi+2] = group->vertices[i].vertex[2];

        Vec3 t = Vec3(group->vertices[i].tangent[0], group->vertices[i].tangent[1], group->vertices[i].tangent[2]);
        Vec3 b = Vec3(group->vertices[i].bitangent[0], group->vertices[i].bitangent[1], group->vertices[i].bitangent[2]);
        Vec3 n = Vec3(group->vertices[i].normal[0], group->vertices[i].normal[1], group->vertices[i].normal[2]);

        //Orthogonalization
        t = glm::normalize(t - n * glm::dot(n, t));
        //Handedness
        if (glm::dot(glm::cross(n, t), b) < 0.0f){
            t = t * -1.0f;
        }

        normals[ni++] = n[0];
        normals[ni++] = n[1];
        normals[ni++] = n[2];

        tangents[vi+0] = t[0];
        tangents[vi+1] = t[1];
        tangents[vi+2] = t[2];

        bitangents[vi+0] = b[0];
        bitangents[vi+1] = b[1];
        bitangents[vi+2] = b[2];

        vi += 3;

        texCoords[ti++] = group->vertices[i].uv[0];
        texCoords[ti++] = 1.0f-group->vertices[i].uv[1]; //Why 1.0f-uv.y? historical reasons, there used to be a bug that
                                                         //loaded textures upside down.
                                                         //For compatibility with old models we keep this

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

    vi = 0;
    for(int i=0; i<group->shapeKeyCount;i++){
        for(int j=0; j<group->shapeKeys[i].numVertices;j++){
            shape_keys[vi+0] = group->shapeKeys[i].vertices[j].vertex[0];
            shape_keys[vi+1] = group->shapeKeys[i].vertices[j].vertex[1];
            shape_keys[vi+2] = group->shapeKeys[i].vertices[j].vertex[2];
            vi += 3;
        }
    }

    glGenBuffers(1, &_vbo[groupIndex]);

    _vboDescriptions[groupIndex].positionSize     = sizeof(GLfloat)*group->numVertices*3;
    _vboDescriptions[groupIndex].normalsSize      = sizeof(GLfloat)*group->numVertices*3;
    _vboDescriptions[groupIndex].tangentsSize     = sizeof(GLfloat)*group->numVertices*3;
    _vboDescriptions[groupIndex].textureCoordSize = sizeof(GLfloat)*group->numVertices*2;
    _vboDescriptions[groupIndex].jointsSize       = sizeof(GLfloat)*group->numVertices*BONE_COUNT;
    _vboDescriptions[groupIndex].weightsSize      = sizeof(GLfloat)*group->numVertices*BONE_COUNT;
    _vboDescriptions[groupIndex].shapeKeysSize    = sizeof(GLfloat)*group->numVertices*3*group->shapeKeyCount;
    _vboDescriptions[groupIndex].totalSize = _vboDescriptions[groupIndex].positionSize
                                             + _vboDescriptions[groupIndex].normalsSize
                                             + _vboDescriptions[groupIndex].tangentsSize
                                             + _vboDescriptions[groupIndex].tangentsSize //bitangents
                                             + _vboDescriptions[groupIndex].textureCoordSize
                                             + _vboDescriptions[groupIndex].jointsSize
                                             + _vboDescriptions[groupIndex].weightsSize
                                             + _vboDescriptions[groupIndex].shapeKeysSize;

    glBindBuffer(GL_ARRAY_BUFFER, _vbo[groupIndex]);
    glBufferData(GL_ARRAY_BUFFER, _vboDescriptions[groupIndex].totalSize, nullptr, GL_STATIC_DRAW);

    /*Copy the data to the buffer*/
    size_t accumulated_size = 0;
    glBufferSubData(GL_ARRAY_BUFFER, accumulated_size, _vboDescriptions[groupIndex].positionSize, vertices);
    accumulated_size += _vboDescriptions[groupIndex].positionSize;
    glBufferSubData(GL_ARRAY_BUFFER, accumulated_size, _vboDescriptions[groupIndex].textureCoordSize, texCoords);
    accumulated_size += _vboDescriptions[groupIndex].textureCoordSize;
    glBufferSubData(GL_ARRAY_BUFFER, accumulated_size, _vboDescriptions[groupIndex].normalsSize, normals);
    accumulated_size += _vboDescriptions[groupIndex].normalsSize;
    glBufferSubData(GL_ARRAY_BUFFER, accumulated_size, _vboDescriptions[groupIndex].tangentsSize, tangents);
    accumulated_size += _vboDescriptions[groupIndex].tangentsSize;
    glBufferSubData(GL_ARRAY_BUFFER, accumulated_size, _vboDescriptions[groupIndex].tangentsSize, bitangents);
    accumulated_size += _vboDescriptions[groupIndex].tangentsSize;
    glBufferSubData(GL_ARRAY_BUFFER, accumulated_size, _vboDescriptions[groupIndex].jointsSize, bone_indices);
    accumulated_size += _vboDescriptions[groupIndex].jointsSize;
    glBufferSubData(GL_ARRAY_BUFFER, accumulated_size, _vboDescriptions[groupIndex].weightsSize, bone_weights);
    accumulated_size += _vboDescriptions[groupIndex].weightsSize;

    _vboDescriptions[groupIndex].beginShapeKeyPointer = accumulated_size;
    for(int i=0; i<group->shapeKeyCount; i++){
        glBufferSubData(GL_ARRAY_BUFFER, accumulated_size, _vboDescriptions[groupIndex].positionSize,
                        (shape_keys+i*group->numVertices*3));
        accumulated_size += _vboDescriptions[groupIndex].positionSize;
    }

    //Set up the indices
    glGenBuffers(1, &_eab[groupIndex]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eab[groupIndex]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexInt)*ii , indices, GL_STATIC_DRAW);

    accumulated_size = 0;
    //Position Attribute
    glVertexAttribPointer(VERTICES__ATTR, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, (GLvoid *)accumulated_size);
    glEnableVertexAttribArray(VERTICES__ATTR);
    accumulated_size += _vboDescriptions[groupIndex].positionSize;

    //Texture coord attribute
    glVertexAttribPointer(TEXCOORDS__ATTR, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)accumulated_size);
    glEnableVertexAttribArray(TEXCOORDS__ATTR);
    accumulated_size += _vboDescriptions[groupIndex].textureCoordSize;

    //Normals attribute
    glVertexAttribPointer(NORMALS__ATTR, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)accumulated_size);
    glEnableVertexAttribArray(NORMALS__ATTR);
    accumulated_size += _vboDescriptions[groupIndex].normalsSize;

    //Tangents and bitangents
    glVertexAttribPointer(TANGENTS__ATTR, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)accumulated_size);
    glEnableVertexAttribArray(TANGENTS__ATTR);
    accumulated_size += _vboDescriptions[groupIndex].tangentsSize;
    glVertexAttribPointer(BITANGENTS__ATTR, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)accumulated_size);
    glEnableVertexAttribArray(BITANGENTS__ATTR);
    accumulated_size += _vboDescriptions[groupIndex].tangentsSize;

    //bone indexes attribute
    glVertexAttribPointer(BONEINDICES__ATTR, BONE_COUNT, GL_FLOAT, GL_FALSE, 0, (GLvoid *)accumulated_size);
    glEnableVertexAttribArray(BONEINDICES__ATTR);
    accumulated_size += _vboDescriptions[groupIndex].jointsSize;

    glVertexAttribPointer(BONEWEIGHTS__ATTR, BONE_COUNT, GL_FLOAT, GL_FALSE, 0, (GLvoid *)accumulated_size);
    glEnableVertexAttribArray(BONEWEIGHTS__ATTR);
    accumulated_size += _vboDescriptions[groupIndex].weightsSize;

    glVertexAttribPointer(SHAPEKEY_SLOT_1_ATTR, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(SHAPEKEY_SLOT_1_ATTR);
    glVertexAttribPointer(SHAPEKEY_SLOT_2_ATTR, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(SHAPEKEY_SLOT_2_ATTR);
    glVertexAttribPointer(SHAPEKEY_SLOT_3_ATTR, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(SHAPEKEY_SLOT_3_ATTR);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //Free all the temporary memory
    delete[] vertices;
    delete[] indices;
    delete[] normals;
    delete[] tangents;
    delete[] bitangents;
    delete[] texCoords;
    delete[] bone_indices;
    delete[] bone_weights;
    delete[] shape_keys;

}

void H3DMesh::setupShapeKey(const std::string &groupName, const std::string &SKname, int slot) {
    int groupIndex = 0;
    int shapeKeyIndex = 0;
    for(;groupIndex < _groupCount; groupIndex++){
        if(!groupName.compare(_groups[groupIndex].name))
            break;
    }
    for(;shapeKeyIndex<_groups[groupIndex].shapeKeyCount;shapeKeyIndex++) {
        if (!SKname.compare(_groups[groupIndex].shapeKeys[shapeKeyIndex].name))
            break;
    }
    glBindVertexArray(_vao[groupIndex]);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo[groupIndex]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eab[groupIndex]);

    size_t SKPointer = _vboDescriptions[groupIndex].totalSize-_vboDescriptions[groupIndex].shapeKeysSize;
    SKPointer += shapeKeyIndex*_vboDescriptions[groupIndex].positionSize;

    glVertexAttribPointer((GLuint)SHAPEKEY_SLOT_1_ATTR+slot-1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3,
                          (GLvoid*)SKPointer);
    glEnableVertexAttribArray((GLuint)SHAPEKEY_SLOT_1_ATTR+slot-1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void H3DMesh::setShapeKeyPercentCallback(std::function<void(float p, int slot)> callback) {
    _shapeKeyPercentCallback = callback;
}

void H3DMesh::setShapeKeyPercent(const std::string &groupName, int slot, float percent) {
    int groupIndex = 0;
    for(;groupIndex < _groupCount; groupIndex++)
        if(!groupName.compare(_groups[groupIndex].name))
            break;

    _groups[groupIndex].sk_slotp[slot-1] = percent;

}

void H3DMesh::setCurrentFrame(float f) {
    _currentFrame = f;
}

void H3DMesh::setCurrentFrame(int f, int animationSlot) {
    _currentFrame = f;
    //setCurrentFrame(f, animationSlot, -1, -1);
}

void H3DMesh::setCurrentFrame(int f, int animationSlot, int start, int end) {
    if(_animationSlotInfo.size() <= (unsigned int)animationSlot) {
        auto animationInfo = new animationSlotInfo;
        animationInfo->currentFrame = f;
        animationInfo->startFrame = start;
        animationInfo->endFrame = end;
        _animationSlotInfo.insert(_animationSlotInfo.begin() + animationSlot, animationInfo);
    }else{
        _animationSlotInfo[animationSlot]->currentFrame = f;
        _animationSlotInfo[animationSlot]->startFrame = start;
        _animationSlotInfo[animationSlot]->endFrame = end;
    }
}

void H3DMesh::recursiveParentTransform(Mat4* transforms, bool* hasParentTransform, h3d_joint* joints, int jointIndex){
    int parentIndex =joints[jointIndex].parentIndex;
    if(parentIndex != -1 && !hasParentTransform[jointIndex]){
        recursiveParentTransform(transforms, hasParentTransform, joints, parentIndex);
        transforms[jointIndex] = transforms[parentIndex] * transforms[jointIndex];
        hasParentTransform[jointIndex] = true;
    }
}

Mat4 H3DMesh::getBoneTransform(h3d_joint* joint, animationSlotInfo* animationInfo) {
    h3d_keyframe* prevKeyframe = nullptr;
    h3d_keyframe* nextKeyframe = nullptr;

    for(int j=0; j<joint->numKeyframes; j++){
        nextKeyframe = &joint->keyframes[j];
        if(j>0){
            prevKeyframe = &joint->keyframes[j-1];
        }else{
            prevKeyframe = nextKeyframe;
        }
        if(nextKeyframe->frame > _currentFrame){//animationInfo->currentFrame){
            break;
        }
    }

    if(nullptr == prevKeyframe || nullptr == nextKeyframe){
        return Mat4(1.0f);
    }
    /*if( (prevKeyframe->frame < animationInfo->startFrame && animationInfo->startFrame >= 0) ||
            (nextKeyframe->frame > animationInfo->endFrame && animationInfo->endFrame >= 0) ){
        return Mat4(1.0f);
    }*/

    if(joint->numKeyframes > 0) {
        float lerpFactor;
        if (nextKeyframe->frame - prevKeyframe->frame == 0) {
            lerpFactor = 0;
        } else {
            /*lerpFactor = ((float)(animationInfo->currentFrame - prevKeyframe->frame)) /
                         ((float)(nextKeyframe->frame - prevKeyframe->frame));*/
            lerpFactor = ((float)(_currentFrame - prevKeyframe->frame)) /
                         ((float)(nextKeyframe->frame - prevKeyframe->frame));

        }
        //std::cout << lerpFactor << std::endl;
        if (lerpFactor < 0)
            lerpFactor = 0;
        if (lerpFactor > 1)
            lerpFactor = 1;


        Vec3 interpolatedTranslation = glm::mix(prevKeyframe->translation, nextKeyframe->translation, lerpFactor);
        Quat interpolatedOrientation = glm::slerp(prevKeyframe->orientation, nextKeyframe->orientation, lerpFactor);

        //Mat4 interpolated = glm::interpolate(prevKeyframe->transform, nextKeyframe->transform, lerpFactor);
        return glm::translate(Mat4(1.0f), interpolatedTranslation) * glm::toMat4(interpolatedOrientation);
        //return nextKeyframe->transform;

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
    if(nullptr == _boneUploadCallback)
        return;

    h3d_armature* armature = &_armatures[group->armatureIndex];

    auto transforms = new Mat4[armature->jointsCount];
    for(int i=0; i<armature->jointsCount; i++){
        transforms[i] = Mat4(1.0f);
    }

    if(!group->isAnimated){ //If not animated the we still have to upload identities if the bones exist
        for(int i=0; i<armature->jointsCount; i++) {
            _boneUploadCallback(i, transforms[i]);
            //glUniformMatrix4fv(bones + i, 1, GL_FALSE, glm::value_ptr(transforms[i]));
        }
        delete[] transforms;
        return;
    }


    for(int i=0; i<armature->jointsCount; i++){
        if(armature->joints[i].numKeyframes == 0)
            continue;

        glm::mat4 transform = Mat4(1.0f);

        /*for(animationSlotInfo* info : _animationSlotInfo) {
             transform *= getBoneTransform(&armature->joints[i], info);
        }*/
        transform = getBoneTransform(&armature->joints[i], nullptr);
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
        _boneUploadCallback(i, transforms[i]);
        //glUniformMatrix4fv(bones + i, 1, GL_FALSE, glm::value_ptr(transforms[i]));
    }
    delete[] hasParentTransform;
    delete[] transforms;

}

void H3DMesh::setBoneUploadCallback(std::function<void(int i, Mat4 transform)> callback) {
    _boneUploadCallback = callback;
}

void H3DMesh::draw() {
    for(int i=0; i < _groupCount; i++){
        handleAnimation(&_groups[i]);
        int materialIndex = _groups[i].materialIndex;
        if( materialIndex >= 0 )
            setMaterial(&_materials[materialIndex]);
        if(_shapeKeyPercentCallback){
            _shapeKeyPercentCallback(_groups[i].sk_slotp[0], 1);
            _shapeKeyPercentCallback(_groups[i].sk_slotp[1], 2);
            _shapeKeyPercentCallback(_groups[i].sk_slotp[2], 3);
        }
        glBindVertexArray(_vao[i]);
        glDrawElements(GL_TRIANGLES, _groups[i].numTriangles*3, GL_UNSIGNED_INT, 0);
    }
}

void H3DMesh::drawGroup(const std::string &name) {
    auto it = _groupMap.find(name);
    if(it != _groupMap.end()) {
        h3d_group* group = it->second;
        int materialIndex = group->materialIndex;
        if( materialIndex >= 0)
            setMaterial(&_materials[materialIndex]);
        glBindVertexArray(_vao[group->index]);
        glDrawElements(GL_TRIANGLES, group->numTriangles*3, GL_UNSIGNED_INT, 0);


    } else
        std::cout << "No group by name: " << name << std::endl;
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

    for(unsigned int i=0;i<material->textureCount;i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        material->texture[i]->bind();
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
            fread(_groups[i].vertices[j].tangent, 3, sizeof(float), fp);
            fread(_groups[i].vertices[j].bitangent, 3, sizeof(float), fp);
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
                //fread(_groups[i].shapeKeys[j].vertices[k].normal, 3, sizeof(float), fp);
            }
        }
        _groups[i].sk_slotp[0] = 0.0f;
        _groups[i].sk_slotp[1] = 0.0f;
        _groups[i].sk_slotp[2] = 0.0f;
        _groups[i].index = i;
        _groupMap[_groups[i].name] = &_groups[i];

    }

    fread(&_materialCount, 1, sizeof(int), fp);
    _materials = new h3d_material[_materialCount];

    std::string folderPath(filename);
    folderPath = folderPath.substr(0, folderPath.find_last_of("/")+1);

    for(int i=0;i<_materialCount;i++){
        //Get the texture file
        fread(&_materials[i].textureCount, 1, sizeof(int), fp);
        _materials[i].textureImage = new char*[_materials[i].textureCount];
        _materials[i].texture = new std::shared_ptr<Texture>[_materials[i].textureCount];
        for(unsigned int j=0; j<_materials[i].textureCount;j++) {
            byte numChars;
            fread(&numChars, 1, sizeof(byte), fp);
            _materials[i].textureImage[j] = new char[numChars + 1];
            if (numChars > 0) {
                fread(_materials[i].textureImage[j], numChars, sizeof(char), fp);
                _materials[i].textureImage[j][numChars] = '\0';

                //Load the texture
                std::string texturePath("./");
                texturePath.assign(folderPath);
                texturePath.append(_materials[i].textureImage[j]);
                _materials[i].texture[j] = ResourceManager::Factory::createTexture(texturePath);
            } else {
                _materials[i].texture[j] = nullptr;
            }
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
                keyframe->translation = Vec3(x, y, z);
                keyframe->orientation = Quat(euler);
                //keyframe->transform = glm::translate(Mat4(1.0f), Vec3(x, y, z)) * glm::toMat4(glm::quat(euler));
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