#version 330 core

in vec2 uv;
in vec3 screen_position;

uniform sampler2D tex;
uniform sampler2D depth;

out vec4 color;

void main() {
    if (screen_position.z >= texture(depth, screen_position.xy).r)
        discard;
    color.rgba = texture(tex, uv).rgba;
    color.a *= 0.5f;
}
