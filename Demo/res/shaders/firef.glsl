#version 330 core

#define DIFFUSE 0
#define AMBIENT 1
#define SPECULAR 2
#define NORMAL 3

in vec2 uv;

uniform sampler2D tex;

out vec4[4] color;

void main() {
    color[DIFFUSE].rgba = texture(tex, uv).rgba;
    color[DIFFUSE].a *= 0.5f;
}
