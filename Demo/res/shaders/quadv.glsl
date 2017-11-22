#version 330 core

in vec3 inPosition;

out vec2 uv;
out vec2 position_modelspace;

uniform mat4 MVP;

void main() {
	vec4 clip_position = MVP* vec4(inPosition, 1.0f);
	gl_Position = clip_position;
	vec3 ndc = clip_position.xyz / clip_position.w;

	uv = ndc.xy*0.5+0.5;

	position_modelspace = inPosition.xy;
}
