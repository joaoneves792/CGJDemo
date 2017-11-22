#version 330 core

in vec2 uv;
in vec2 position_modelspace;

uniform sampler2D renderedTexture;
uniform float offset;

out vec4 color;

void main() {
    vec2 texcoord = uv;
    texcoord.x += sin(texcoord.y * 4*2*3.14159 + offset) / 100;
    color.rgb = texture(renderedTexture, texcoord).rgb;

    color.a = 1.0f-length(position_modelspace);

    //color = vec4(texture(renderedTexture, (position_modelspace+1)/2).rgb, 1.0f);
}
