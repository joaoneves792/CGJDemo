#version 330 core

in vec2 uv;

uniform sampler2D renderedTexture;

out vec4 color;

void main() {
    color.rgb = texture(renderedTexture, uv).rgb;
    color.a = 1.0f;
}
