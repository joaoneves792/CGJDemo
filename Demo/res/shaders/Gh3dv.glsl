#version 330 core

#define MAX_LIGHTS 20

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;
in vec3 inJointIndex;
in vec3 inJointWeight;

out vec3 position_worldspace;
out float z_clipspace;
out vec3 eyeDirection_cameraspace;
out vec3 eyeDirection_worldspace;
out vec2 texture_coord_from_vshader;
out vec3 normal_cameraspace;
out vec3 normal_worldspace;
out vec3 lightDirection_cameraspace[MAX_LIGHTS];

uniform vec3 lightPosition_worldspace[MAX_LIGHTS];
uniform vec4 lightCone[MAX_LIGHTS]; //xyz->direction //w->angle
uniform int lightsEnabled[MAX_LIGHTS];

uniform mat4 Model;
uniform mat4 View;
uniform mat4 NormalMatrix;
uniform mat4 MVP;

void main() {
    vec4 position = vec4(inPosition, 1.0f);

    /*Calculate normals*/
    normal_cameraspace = (NormalMatrix * vec4(inNormal, 0)).xyz;
    normal_worldspace = (Model * vec4(inNormal, 0.0f)).xyz; //Assuming there is no scale

	/*Position in worldspace and cameraspace*/
	position_worldspace = (Model * position).xyz;
    vec3 position_cameraspace = (View * vec4(position_worldspace, 1.0f)).xyz;

    /*Eye direction in worldspace*/
    mat4 MV = View;
    mat3 rotMat = mat3(MV);
    vec3 d = vec3(MV[3]);
    vec3 eyePos = -d * rotMat;
    eyeDirection_worldspace = position_worldspace - eyePos;

	/*Eye direction in cameraspace*/
	eyeDirection_cameraspace = vec3(0,0,0) - position_cameraspace;

    vec4 position_clipspace = MVP * position;
    z_clipspace = position_clipspace.z;
	gl_Position = position_clipspace;

	texture_coord_from_vshader = inTexCoord;
	
	for(int i=0; i<MAX_LIGHTS; i++){
		if(lightsEnabled[i] == 0)
			continue;
		vec3 lightPosition_cameraspace = (View * vec4(lightPosition_worldspace[i].xyz, 1.0f)).xyz;
		lightDirection_cameraspace[i] = lightPosition_cameraspace - position_cameraspace;
	}

}
