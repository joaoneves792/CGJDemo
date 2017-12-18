#version 330 core

in vec2 uv;

out vec4 out_color;

uniform sampler2D background;
uniform sampler2D foreground;

void main() {
    vec4 dest = texture(background, uv);
    vec4 src = texture(foreground, uv);
    out_color.r = src.r*src.a+dest.r*(1-src.a);
    out_color.g = src.g*src.a+dest.g*(1-src.a);
    out_color.b = src.b*src.a+dest.b*(1-src.a);
    out_color.a = 1.0f;
}
