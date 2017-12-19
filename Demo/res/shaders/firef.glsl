#version 330 core

#define DIFFUSE 0
#define AMBIENT 1
#define SPECULAR 2
#define NORMAL 3
#define PARTICLES 4

in vec2 uv;

uniform sampler2D tex;

out vec4[5] color;

void main() {
    color[PARTICLES].rgba = texture(tex, uv).rgba;
    color[PARTICLES].a *= 0.5f;
}
