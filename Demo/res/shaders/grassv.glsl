#version 330 core

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec2 texture_coord_from_vshader;
out vec3 normal_cameraspace;
out vec3 position_modelspace;

uniform mat4 NormalMatrix;
uniform mat4 MVP;

uniform float furLength;
uniform float time;

void main() {
    vec4 position = vec4(inPosition + inNormal*furLength, 1.0f);

    float k =  pow(furLength, 3);
    position = position + vec4(2.0f, -2.0f, 2.0f, 0.0f)*k;

    /*Calculate normals*/
    normal_cameraspace = ((NormalMatrix *  vec4(inNormal, 0.0f)).xyz);

    vec4 position_clipspace = MVP * position;
	gl_Position = position_clipspace;

	texture_coord_from_vshader = inTexCoord;
	position_modelspace = position.xyz;
}
