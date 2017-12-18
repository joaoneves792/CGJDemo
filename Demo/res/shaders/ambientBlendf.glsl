#version 330 core

in vec2 uv;

out vec4 out_color;

uniform sampler2D frame;
uniform sampler2D ambient;
uniform sampler2D ambientOcclusion;

void main() {
    vec3 color = texture(frame, uv).rgb;
    vec3 ambientColor = texture(ambient, uv).rgb;
    float occlusion = texture(ambientOcclusion, uv).r;

    out_color.rgb = (color + ambientColor)*occlusion;
    out_color.a = 1.0f;
}
