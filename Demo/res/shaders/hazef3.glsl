#version 330 core

in vec2 uv;
in vec2 position_modelspace;
in float life;

uniform sampler2D renderedTexture;
uniform float black;

out vec4 color;

#define PI 3.14159f
#define AMPLITUDE 1.0f/800.0f
#define FREQUENCY PI/2.0f
#define PHASE 100.0f

#define X_AMPLITUDE 0.4f
#define Y_AMPLITUDE 0.5f

#define HARMONICS_A 1.2f
#define HARMONICS_B 1.5f
#define HARMONICS_N 6

#define NEXT_TEXEL_STEP 0.0025f

float blur_kernel[9] = float[](0.05f, 0.15f, 0.05f,
                               0.15f, 0.15f, 0.15f,
                               0.05f, 0.15f, 0.05f);


vec3 convolute(float[9] kernel, vec2 pos){
    vec3 t[9] = vec3[](
        texture(renderedTexture, vec2(pos.x-NEXT_TEXEL_STEP, pos.y+NEXT_TEXEL_STEP)).rgb,
        texture(renderedTexture, vec2(pos.x, pos.y+NEXT_TEXEL_STEP)).rgb,
        texture(renderedTexture, vec2(pos.x+NEXT_TEXEL_STEP, pos.y+NEXT_TEXEL_STEP)).rgb,

        texture(renderedTexture, vec2(pos.x-NEXT_TEXEL_STEP, pos.y)).rgb,
        texture(renderedTexture, vec2(pos.x, pos.y)).rgb,
        texture(renderedTexture, vec2(pos.x+NEXT_TEXEL_STEP, pos.y)).rgb,

        texture(renderedTexture, vec2(pos.x-NEXT_TEXEL_STEP, pos.y-NEXT_TEXEL_STEP)).rgb,
        texture(renderedTexture, vec2(pos.x, pos.y-NEXT_TEXEL_STEP)).rgb,
        texture(renderedTexture, vec2(pos.x+NEXT_TEXEL_STEP, pos.y-NEXT_TEXEL_STEP)).rgb
    );

    return kernel[8]*t[0] + kernel[7]*t[1] + kernel[6]*t[2] +
           kernel[5]*t[3] + kernel[4]*t[4] + kernel[3]*t[5] +
           kernel[2]*t[6] + kernel[1]*t[7] + kernel[0]*t[8];
}

float harmonic_sin(int i, float a, float f){
         return (AMPLITUDE*sin(pow(HARMONICS_B, i)*FREQUENCY*a + PHASE*f))/pow(HARMONICS_A, i);
}

void main() {
    vec2 pos = position_modelspace;
    vec2 noise = vec2(0.0f, 0.0f);
    for(int i=0; i<HARMONICS_N; i++){
        float hs = harmonic_sin(i, pos.x, life);
        noise.x += X_AMPLITUDE*hs;
        noise.y += Y_AMPLITUDE*hs;
    }

    color.rgb = convolute(blur_kernel, uv+noise);
    color.a = 1.0f-length(position_modelspace)*2;

    if(black > 0.0){
            color.rgb = vec3(0.0f, 0.0f, 0.0f);
    }
}
