#version 330 core

in vec2 uv;

out vec4[5] out_color;

uniform sampler2D background;
uniform int renderTarget;

void main() {
    out_color[renderTarget].rgb = texture(background, uv).rgb;
    out_color[renderTarget].a = 1.0f;
}
