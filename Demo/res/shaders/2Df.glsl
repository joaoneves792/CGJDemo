#version 330 core

in vec2 uv;

uniform sampler2D renderedTexture;

out vec4 color;

void main() {
    color.rgba = texture(renderedTexture, uv).rgba;
}
