#version 330 core

in vec2 uv;

uniform sampler2D ssaoTexture;

out vec4 color;

float blur_kernel[9] = float[](0.05f, 0.15f, 0.05f,
                               0.15f, 0.15f, 0.15f,
                               0.05f, 0.15f, 0.05f);

vec3 convolute(float[9] kernel, vec2 pos, vec2 texelSize){
    float up = clamp(pos.y + texelSize.y, 0.0f, 1.0f);
    float down = clamp(pos.y - texelSize.y, 0.0f, 1.0f);
    float left = clamp(pos.x - texelSize.x, 0.0f, 1.0f);
    float right = clamp(pos.x + texelSize.x, 0.0f, 1.0f);
    vec3 t[9] = vec3[](
        texture(ssaoTexture, vec2(left, up)).rgb,
        texture(ssaoTexture, vec2(pos.x, up)).rgb,
        texture(ssaoTexture, vec2(right, up)).rgb,

        texture(ssaoTexture, vec2(left, pos.y)).rgb,
        texture(ssaoTexture, vec2(pos.x, pos.y)).rgb,
        texture(ssaoTexture, vec2(right, pos.y)).rgb,

        texture(ssaoTexture, vec2(left, down)).rgb,
        texture(ssaoTexture, vec2(pos.x, down)).rgb,
        texture(ssaoTexture, vec2(right, down)).rgb
    );

    return kernel[8]*t[0] + kernel[7]*t[1] + kernel[6]*t[2] +
           kernel[5]*t[3] + kernel[4]*t[4] + kernel[3]*t[5] +
           kernel[2]*t[6] + kernel[1]*t[7] + kernel[0]*t[8];
}

void main() {
    vec2 texelSize = 2.0f / vec2(textureSize(ssaoTexture, 0));
    float occlusion = convolute(blur_kernel, uv, texelSize).r;

    color = vec4(occlusion, occlusion, occlusion, 1.0f);
}

