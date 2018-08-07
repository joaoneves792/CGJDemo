#version 330 core

#define DIFFUSE 0
#define AMBIENT 1
#define SPECULAR 2
#define NORMAL 3

in vec3 position_worldspace;
in vec3 normal_cameraspace;

out vec4[4] G_output;

void main() {
    G_output[DIFFUSE].rgba = vec4(0.5f, 0.5f, 0.0f, 1.0f);
    G_output[AMBIENT].rgba = vec4(0.5f, 0.5f, 0.0f, 1.0f);
    G_output[SPECULAR].xyz = vec3(1.0f, 1.0f, 1.0f);
    G_output[SPECULAR].w = 0.2f;
    G_output[NORMAL] = vec4(normalize(normal_cameraspace), 0.0f);
}
