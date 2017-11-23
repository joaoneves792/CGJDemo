#version 330 core

#define MAX_LIGHTS 20

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;
in vec3 inJointIndex;
in vec3 inJointWeight;

out vec3 position_worldspace;
out vec3 eyeDirection_cameraspace;
out vec2 texture_coord_from_vshader;
out vec3 normal_cameraspace;
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

    normal_cameraspace = (NormalMatrix * vec4(inNormal, 0)).xyz;
	position_worldspace = (Model * position).xyz;
    vec3 position_cameraspace = (View * Model * position).xyz;

	gl_Position = MVP * position;

	texture_coord_from_vshader = inTexCoord;
	
	for(int i=0; i<MAX_LIGHTS; i++){
		if(lightsEnabled[i] == 0)
			continue;
		vec3 lightPosition_cameraspace = (View * vec4(lightPosition_worldspace[i].xyz, 1.0f)).xyz;
		eyeDirection_cameraspace = vec3(0,0,0) - position_cameraspace;
		lightDirection_cameraspace[i] = lightPosition_cameraspace - position_cameraspace;
	}

}
