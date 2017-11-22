#version 330 core

in vec4 inPosition;
in vec2 inTexCoord;

out vec2 texture_coord_from_vshader;

uniform mat4 MVP;

void main(void){
	gl_Position = MVP * inPosition;
    texture_coord_from_vshader = inTexCoord;
}

