#version 330 core

in vec2 texture_coord_from_vshader;

out vec4 outColor;

uniform sampler2D texture_sampler;

void main(void){
    outColor = vec4(texture(texture_sampler, texture_coord_from_vshader).rgb, 1.0f);
}

