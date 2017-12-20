#version 330 core

#define PARTICLES 4

in vec2 uv;
in vec2 position_modelspace;
in float life;

uniform sampler2D renderedTexture;
uniform sampler2D noiseTexture;

out vec4[5] color;

#define PI 3.14159f
#define AMPLITUDE 1.0f/800.0f
#define FREQUENCY PI/2.0f
#define PHASE 100.0f

#define X_AMPLITUDE 0.1f
#define Y_AMPLITUDE 0.2f

#define HARMONICS_A 1.2f
#define HARMONICS_B 1.5f
#define HARMONICS_N 4

#define NEXT_TEXEL_STEP 0.0025f
#define NEXT_TEXEL_STEP_BLUR 0.001f

#define NOISE_FREQUENCY 64.0f
#define NOISE_PERIOD  40.0f
#define NOISE_AMPLITUDE 0.7f

float blur_kernel[9] = float[](0.05f, 0.15f, 0.05f,
                               0.15f, 0.15f, 0.15f,
                               0.05f, 0.15f, 0.05f);

vec3 convolute(float[9] kernel, vec2 pos){
    vec3 t[9] = vec3[](
        texture(renderedTexture, vec2(pos.x-NEXT_TEXEL_STEP_BLUR, pos.y+NEXT_TEXEL_STEP_BLUR)).rgb,
        texture(renderedTexture, vec2(pos.x, pos.y+NEXT_TEXEL_STEP_BLUR)).rgb,
        texture(renderedTexture, vec2(pos.x+NEXT_TEXEL_STEP_BLUR, pos.y+NEXT_TEXEL_STEP_BLUR)).rgb,

        texture(renderedTexture, vec2(pos.x-NEXT_TEXEL_STEP_BLUR, pos.y)).rgb,
        texture(renderedTexture, vec2(pos.x, pos.y)).rgb,
        texture(renderedTexture, vec2(pos.x+NEXT_TEXEL_STEP_BLUR, pos.y)).rgb,

        texture(renderedTexture, vec2(pos.x-NEXT_TEXEL_STEP_BLUR, pos.y-NEXT_TEXEL_STEP_BLUR)).rgb,
        texture(renderedTexture, vec2(pos.x, pos.y-NEXT_TEXEL_STEP_BLUR)).rgb,
        texture(renderedTexture, vec2(pos.x+NEXT_TEXEL_STEP_BLUR, pos.y-NEXT_TEXEL_STEP_BLUR)).rgb
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
    float hs = harmonic_sin(0, pos.x, life)+
               harmonic_sin(1, pos.x, life)+
               harmonic_sin(2, pos.x, life)+
               harmonic_sin(3, pos.x, life);

    vec2 wave_offset = vec2(X_AMPLITUDE*hs, Y_AMPLITUDE*hs);

    float noise = texture(noiseTexture, vec2(pos.x+2*life, pos.y)).r-0.5f;
    vec2 noise_offset = vec2(cos(noise*NOISE_FREQUENCY), sin(noise*NOISE_FREQUENCY)) * NOISE_AMPLITUDE * NEXT_TEXEL_STEP;
    vec3 noise_component = texture(renderedTexture, uv+wave_offset+noise_offset).rgb;
    vec3 blur_component = convolute(blur_kernel, uv+wave_offset);

    color[PARTICLES].rgb = 0.6f*noise_component+0.4*blur_component;
    color[PARTICLES].a = 1.0f-smoothstep(0.7, 1.0f, length(pos));
}
