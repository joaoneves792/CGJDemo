#version 330 core

in vec2 uv;
in vec2 position_modelspace;
in float life;

uniform sampler2D renderedTexture;

out vec4 color;

#define PI 3.14159

void main() {
    vec2 texcoord = uv;
    texcoord.x += sin(texcoord.y * PI + life*100.0f) / 500.0f;
    color.rgb = texture(renderedTexture, texcoord).rgb;

    color.a = 1.0f-length(position_modelspace)*2;

    //color = vec4(texture(renderedTexture, (position_modelspace+1)/2).rgb, 1.0f);
    //color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
