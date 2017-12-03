#version 330 core

in vec3 inPosition;

out vec2 uv;

uniform mat4 MVP;

void main() {
	gl_Position = MVP * vec4(inPosition, 1.0f);
	uv.x = (inPosition.x+1)/2;
	uv.y = (inPosition.y+1)/-2;
}
