#version 330 core

in vec3 inPosition;

out vec2 uv;

uniform mat4 MVP;

void main() {
	vec4 clip_position = MVP* vec4(inPosition, 1.0f);
	gl_Position = clip_position;
	uv = (clip_position.xy+1)/2;
}
