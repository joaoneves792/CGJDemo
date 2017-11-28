#version 330 core
in vec3 vertex;
in vec3 position;
in float life;

out vec2 uv;

uniform mat4 MVP;

void main() {
    uv = ((vertex.xy*2.0f)+1.0f)/2.0f;
    gl_Position = MVP * vec4(vertex+position, 1.0f);
}
