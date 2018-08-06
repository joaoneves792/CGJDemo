#version 330 core

#define DIFFUSE 0
#define AMBIENT 1
#define SPECULAR 2
#define NORMAL 3

in vec2 texture_coord_from_vshader;
in vec3 normal_cameraspace;

out vec4[4] G_output;

/*Material Properties*/
uniform sampler2D base;
uniform sampler2D noise;
uniform float furLength;
uniform float uvScale;

void main() {
    vec4 matDiffuse;
    if(furLength == 0.0f)
        matDiffuse = texture(base, texture_coord_from_vshader);
    else
        matDiffuse = texture(noise, texture_coord_from_vshader*uvScale);

    if(matDiffuse.a == 0.0f)
        discard;


    G_output[DIFFUSE] = matDiffuse;

    G_output[AMBIENT].rgb = matDiffuse.rgb*0.3f;
    G_output[AMBIENT].a = 1.0f;

    G_output[SPECULAR].xyz = vec3(0.0f);
    G_output[SPECULAR].w = 0.0f;

    G_output[NORMAL] = vec4(normal_cameraspace, 1.0f);
}
