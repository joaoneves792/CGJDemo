#version 330 core

in vec3 TexCoords;

out vec4 outColor;

uniform samplerCube skybox;
uniform float brightness;

void main()
{
    outColor = texture(skybox, TexCoords)*brightness;
}