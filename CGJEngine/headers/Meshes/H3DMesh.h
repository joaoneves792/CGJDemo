/*
 *  Created on: Jul 7, 2017
 */


#ifndef _H3DMESH_H_
#define _H3DMESH_H_

#include <GL/glew.h>
#include <string>
#include <vector>
#include <functional>

#include "Meshes/Mesh.h"
#include "glm_wrapper.h"

#ifndef RC_INVOKED
#pragma pack(push,1)
#endif


#ifndef byte
typedef unsigned char byte;
#endif // byte

#ifndef word
typedef unsigned short word;
#endif // word

#define BONE_COUNT 3

typedef struct
{
    char    id[3];                                     // always "H3D"
    char    version;                                   // 1
} h3d_header;

typedef struct
{
    float   vertex[3];
	float 	normal[3];
	float   tangent[3];
	float   bitangent[3];
	float 	uv[2];
	int     boneID[BONE_COUNT];                                    // -1 = no bone
    float   weight[BONE_COUNT];
} h3d_vertex;

typedef struct
{
    int    vertexIndices[3];                           //
} h3d_triangle;

typedef struct
{
	int             frame;
	float           position[3];						//local reference
	float           rotation[3];						//local reference
    Mat4		    transform;
}h3d_keyframe;

typedef struct
{
    char*           name;                           	//
    int 			parentIndex; 						// -1 if no parent
    float           position[3];						//world reference
	float           rotation[3];                        //eulerXYZ	//world reference

    int             numKeyframes;                       //
    h3d_keyframe*  	keyframes;

	Mat4 			bindPose;							//Filled when loading
	Mat4			invBindPose;						//Filled when loading

} h3d_joint;


typedef struct
{
    char*           name;
	int 			jointsCount;
	h3d_joint*		joints;

}h3d_armature;

typedef struct
{
	char* 			name;
	int 			numVertices;
	h3d_vertex*		vertices;
}h3d_shape_key;

typedef struct
{
    char*           name;
    int             numTriangles;
    h3d_triangle*  	triangles;
	int 			numVertices;
	h3d_vertex* 	vertices;
    int             materialIndex;                      // -1 = no material
	char 			isAnimated; 						// 0 if no armature
	char* 			armatureName;						// if isAnimated is 1
	int 			armatureIndex;

	int 			shapeKeyCount;
	h3d_shape_key*	shapeKeys;
	float			sk_slotp[3];						//Percentage of shapekey slot influence
} h3d_group;

typedef struct
{
    char*           name;
    float           ambient;                             //
    float           diffuse[4];                         //
    float           specular[4];                        //
    float           emissive[4];                        //
    float           shininess;                          // 0.0f - 128.0f
    float           transparency;                       // 0.0f - 1.0f
    unsigned int    textureCount;
	char**          textureImage;
	GLint* 			textureId;
} h3d_material;


//#include <poppack.h>
#ifndef RC_INVOKED
#pragma pack(pop)
#endif

typedef GLuint indexInt;

typedef struct{
	size_t positionSize;
	size_t textureCoordSize;
	size_t normalsSize;
    size_t tangentsSize;
    size_t jointsSize;
	size_t weightsSize;
	size_t shapeKeysSize;
	size_t beginShapeKeyPointer;
	size_t totalSize;
}h3d_vboDescription;

typedef struct{
	int currentFrame;
	int startFrame;
	int endFrame;
}animationSlotInfo;

class H3DMesh : public Mesh{
private:
	h3d_group* _groups;
	int _groupCount;
	h3d_material* _materials;
	int _materialCount;
	h3d_armature* _armatures;
	int _armatureCount;

	GLuint* _vao;
	GLuint* _vbo;
	GLuint* _eab;

	h3d_vboDescription* _vboDescriptions;

	std::function<void(float ambient, float* diffuse, float* specular,
					   float* emissive, float shininess, float transparency)> _uploadMaterialCallback;

	std::function<void(int i, Mat4 transform)> _boneUploadCallback;
	std::function<void(float p, int slot)> _shapeKeyPercentCallback;

	bool _isAnimated;
	std::vector<animationSlotInfo*> _animationSlotInfo;

public:
	H3DMesh();
	H3DMesh(const std::string& filename);
	virtual ~H3DMesh();

	void loadFromFile(const std::string& filename);
	void Clear();

	void freeMeshData();
	void prepare();
	void unload();
	void draw();

	void setCurrentFrame(int f, int animationSlot);
	void setCurrentFrame(int f, int animationSlot, int start, int end);

	void setMaterialUploadCallback(std::function<void(float ambient, float* diffuse, float* specular,
					   float* emissive, float shininess, float transparency)> callback );
	void setBoneUploadCallback(std::function<void(int i, Mat4 transform)> callback);
	void setShapeKeyPercentCallback(std::function<void(float p, int slot)> callback);
	void setupShapeKey(const std::string& groupName, const std::string& SKname, int slot);
	void setShapeKeyPercent(const std::string& groupName, int slot, float percent);

private:
	void prepareGroup(h3d_group* group, unsigned int groupIndex);
	void setMaterial(h3d_material *material);
	void recursiveParentTransform(Mat4* transforms, bool* hasParentTransform, h3d_joint* joints, int jointIndex);
	Mat4 recursiveBindPose(h3d_joint* joints, int i);
    Mat4 getBindPose(h3d_joint* joint);
	void handleAnimation(h3d_group* group);
    Mat4 getBoneTransform(h3d_joint* joint, animationSlotInfo* animationInfo);
};

#endif // _H3DMESH_H_
