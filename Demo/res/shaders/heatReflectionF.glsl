#version 330 core

in vec2 uv;
in vec3 eyeDirection_worldspace;

uniform sampler2D map;
uniform sampler2D environment;

out vec4 color;

void main() {
    float factor = texture(map, uv).r;
    color.rgb = texture(environment, uv).rgb*factor;
    color.a = factor-0.3;
    //color.a = 1.0f;
}
