#version 330 core

in vec4 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;
in vec3 inJointIndex;
in vec3 inJointWeight;

out vec3 fragmentPosition;
out vec3 position_worldspace;
out vec3 normal_cameraspace;
out vec3 normal_worldspace;
out vec3 eyeDirection_cameraspace;
out vec3 eyeDirection_worldspace;
out vec3 lightDirection_cameraspace;

out vec2 texture_coord_from_vshader;

uniform mat4 MVP;
uniform mat4 View;
uniform mat4 Model;

void main(void){
	fragmentPosition = inPosition.xyz;
	vec4 position = inPosition;

	gl_Position = MVP * position;

	position_worldspace = (Model * position).xyz;

    mat4 MV = View*Model;

   	vec3 vertexPosition_cameraspace = (MV * position).xyz;
   	eyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

    vec3 lightPosition_worldspace = vec3(5, 200, 50);
    vec3 lightPosition_cameraspace = (View * vec4(lightPosition_worldspace.xyz, 1.0f)).xyz;
    lightDirection_cameraspace = lightPosition_cameraspace + eyeDirection_cameraspace;

    // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
    normal_worldspace = (Model * vec4(inNormal, 0)).xyz;
    normal_cameraspace = (View * vec4(normal_worldspace, 0)).xyz;


    mat3 rotMat = mat3(MV);
    vec3 d = vec3(MV[3]);
    vec3 eyePos = -d * rotMat;
    eyeDirection_worldspace = normalize(eyePos - position_worldspace);

    texture_coord_from_vshader = inTexCoord;
}

