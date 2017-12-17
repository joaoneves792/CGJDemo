#version 330 core

in vec2 uv;

uniform sampler2D ssaoTexture;
uniform sampler2D renderedScene;
uniform float toggle;


out vec4 color;

float blur_kernel[9] = float[](0.05f, 0.15f, 0.05f,
                               0.15f, 0.15f, 0.15f,
                               0.05f, 0.15f, 0.05f);

vec3 convolute(float[9] kernel, vec2 pos, vec2 texelSize){
    vec3 t[9] = vec3[](
        texture(ssaoTexture, vec2(pos.x-texelSize.x, pos.y+texelSize.y)).rgb,
        texture(ssaoTexture, vec2(pos.x, pos.y+texelSize.y)).rgb,
        texture(ssaoTexture, vec2(pos.x+texelSize.x, pos.y+texelSize.y)).rgb,

        texture(ssaoTexture, vec2(pos.x-texelSize.x, pos.y)).rgb,
        texture(ssaoTexture, vec2(pos.x, pos.y)).rgb,
        texture(ssaoTexture, vec2(pos.x+texelSize.x, pos.y)).rgb,

        texture(ssaoTexture, vec2(pos.x-texelSize.x, pos.y-texelSize.y)).rgb,
        texture(ssaoTexture, vec2(pos.x, pos.y-texelSize.y)).rgb,
        texture(ssaoTexture, vec2(pos.x+texelSize.x, pos.y-texelSize.y)).rgb
    );

    return kernel[8]*t[0] + kernel[7]*t[1] + kernel[6]*t[2] +
           kernel[5]*t[3] + kernel[4]*t[4] + kernel[3]*t[5] +
           kernel[2]*t[6] + kernel[1]*t[7] + kernel[0]*t[8];
}

void main() {
    vec2 texelSize = 2.0f / vec2(textureSize(ssaoTexture, 0));
    vec3 occlusion = convolute(blur_kernel, uv, texelSize).rgb;

    if(toggle < 1.0f){
        color = vec4(texture(renderedScene, uv).rgb*occlusion.r, 1.0f);
    }else if (toggle < 2.0f){
        color = vec4(occlusion, 1.0f);
    }else{
        color = texture(renderedScene, uv);
    }
}

