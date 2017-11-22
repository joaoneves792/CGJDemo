#version 330 core

#define MAX_LIGHTS 10

in vec4 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;
in vec3 inJointIndex;
in vec3 inJointWeight;

out vec3 position_worldspace;
out vec2 texture_coord_from_vshader;
out vec3 normal_cameraspace;
out vec3 eyeDirection_cameraspace;
out vec3 lightDirection_cameraspace[MAX_LIGHTS];

uniform mat4 Model;
uniform mat4 View;
uniform mat4 MVP;

void main() {
    vec4 newPosition;
    newPosition = position;
    normal_cameraspace = (View * Model * vec4(normal, 0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.

	position_worldspace = (Model * newPosition).xyz;

	gl_Position = MVP * vec4(newPosition.xyz, 1.0f);
	texture_coord_from_vshader = texture_coord;
	
	vec3 vertexPosition_cameraspace = (View * Model * newPosition).xyz;
	eyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	for(int i=0; i<MAX_LIGHTS; i++){
		if(lightsEnabled[i] == 0)
			continue;
		vec3 lightPosition_cameraspace = (View * vec4(lightPosition_worldspace[i].xyz, 1)).xyz;
		lightDirection_cameraspace[i] = lightPosition_cameraspace + eyeDirection_cameraspace;
	}

}
