#version 330 core

#define DIFFUSE 0
#define AMBIENT 1
#define SPECULAR 2
#define NORMAL 3

in vec3 TexCoords;

out vec4[4] outColor;

uniform samplerCube skybox;
uniform float brightness;

void main()
{
    outColor[DIFFUSE] = texture(skybox, TexCoords)*brightness;
    outColor[AMBIENT] = outColor[DIFFUSE];

    outColor[SPECULAR] = vec4(0.0f);
    outColor[NORMAL] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}