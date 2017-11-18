#version 330 core

in vec3 inPosition;
in vec3 inNormal;

out vec3 fragmentPosition;
out vec3 position_worldspace;
out vec3 normal_cameraspace;
out vec3 eyeDirection_cameraspace;
out vec3 lightDirection_cameraspace;

uniform mat4 MVP;
uniform mat4 View;
uniform mat4 Model;

void main(void){
	fragmentPosition = inPosition;
	vec4 position = vec4(inPosition.xyz, 1.0f);

	gl_Position = MVP * position;

	position_worldspace = (Model * position).xyz;

   	vec3 vertexPosition_cameraspace = (View * Model * position).xyz;
   	eyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

    vec3 lightPosition_worldspace = vec3(5, 20, 5);
    vec3 lightPosition_cameraspace = (View * vec4(lightPosition_worldspace.xyz, 1.0f)).xyz;
    lightDirection_cameraspace = lightPosition_cameraspace + eyeDirection_cameraspace;

    // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
    normal_cameraspace = (View * Model * vec4(inNormal, 0)).xyz;
}

