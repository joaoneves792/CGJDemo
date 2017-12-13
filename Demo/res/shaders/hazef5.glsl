#version 330 core

in vec2 uv;
in vec2 position_modelspace;
in float life;

uniform sampler2D renderedTexture;
uniform sampler2D noiseTexture;
uniform float noise_blur_black;

out vec4 color;

#define PI 3.14159f
#define AMPLITUDE 1.0f/800.0f
#define FREQUENCY PI/2.0f
#define PHASE 100.0f

#define X_AMPLITUDE 0.2f
#define Y_AMPLITUDE 0.3f

#define HARMONICS_A 1.2f
#define HARMONICS_B 1.5f
#define HARMONICS_N 4

#define NEXT_TEXEL_STEP 0.0025f

#define NOISE_FREQUENCY 64.0f
#define NOISE_PERIOD  40.0f
#define NOISE_AMPLITUDE 0.7f

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
    vec2 wave_offset = vec2(0.0f, 0.0f);
    for(int i=0; i<HARMONICS_N; i++){
        float hs = harmonic_sin(i, pos.x, life);
        wave_offset.x += X_AMPLITUDE*hs;
        wave_offset.y += Y_AMPLITUDE*hs;
    }



    float noise = texture(noiseTexture, vec2(pos.x+life, pos.y)).r-0.5f;
    noise *= 30;
    vec2 noise_offset = vec2(cos(noise), sin(noise)) * NOISE_AMPLITUDE * NEXT_TEXEL_STEP;

    vec2 waved_uv = clamp(uv+wave_offset, NEXT_TEXEL_STEP, 1.0f-NEXT_TEXEL_STEP);
    vec2 noise_uv = clamp(waved_uv+noise_offset, 0.0f, 1.0f);

    vec3 noise_component = texture(renderedTexture, noise_uv).rgb;
    vec3 blur_component = convolute(blur_kernel, waved_uv);
    if(noise_blur_black == 0.0){
        color.rgb = 0.6f*noise_component+0.4*blur_component;
    }else if(noise_blur_black == 1.0f){
        color.rgb = noise_component;
    }else if(noise_blur_black == 2.0f){
        color.rgb = blur_component;
    }else{
            color.rgb = vec3(0.0f, 0.0f, 0.0f);
    }

    color.a = 1.0f-length(position_modelspace)*2;
}
