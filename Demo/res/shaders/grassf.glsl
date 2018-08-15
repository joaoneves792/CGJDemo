#version 330 core

#define DIFFUSE 0
#define AMBIENT 1
#define SPECULAR 2
#define NORMAL 3

in vec2 texture_coord_from_vshader;
in vec3 normal_cameraspace;
in vec3 position_modelspace;

out vec4[4] G_output;

/*Material Properties*/
uniform sampler2D base;
uniform sampler2D grass;

uniform float furLength;
uniform float uvScale;
uniform float time;

#define PI 3.14159f
#define AMPLITUDE 1.0f/200.0f
#define FREQUENCY PI/4
#define PHASE 0.5f

#define X_AMPLITUDE 0.1f
#define Y_AMPLITUDE 0.3f

#define HARMONICS_A 0.8f
#define HARMONICS_B 1.5f

#define NEXT_TEXEL_STEP 0.0025f

#define NOISE_FREQUENCY 20.0f
#define NOISE_PERIOD  40.0f
#define NOISE_AMPLITUDE 0.9f

float harmonic_sin(int i, float a, float f){
         return (AMPLITUDE*sin(pow(HARMONICS_B, i)*FREQUENCY*a + PHASE*f))/pow(HARMONICS_A, i);
}

void main() {
    vec4 matDiffuse;
    vec3 pos = position_modelspace;
    vec2 uv = texture_coord_from_vshader;
    float hs = harmonic_sin(0, pos.x, time)+
               harmonic_sin(1, pos.x, time)+
               harmonic_sin(2, pos.x, time)+
               harmonic_sin(3, pos.x, time);

    vec2 wave_offset = vec2(X_AMPLITUDE*hs*furLength, Y_AMPLITUDE*hs*furLength);
    //float noise = texture(noiseTexture, vec2(pos.x+life, pos.y)).r-0.5f;
    //vec2 noise_offset = vec2(cos(noise*NOISE_FREQUENCY), sin(noise*NOISE_FREQUENCY)) * NOISE_AMPLITUDE * NEXT_TEXEL_STEP;

    //vec2 waved_uv = uv+wave_offset;
    //vec2 waved_uv = vec2(uv.x+0.02*sin(time)*furLength, uv.y);
    vec2 waved_uv = vec2(uv.x+hs*furLength, uv.y+hs*furLength);

    if(furLength == 0.0f)
        matDiffuse = texture(base, texture_coord_from_vshader);
    else
        matDiffuse = texture(grass, waved_uv); //*uvScale);

    if(matDiffuse.a <= 0.0f)
        discard;


    G_output[DIFFUSE] = matDiffuse;

    G_output[AMBIENT].rgba = matDiffuse.rgba*0.3f;
    //G_output[AMBIENT].a = 1.0f;

    G_output[SPECULAR].xyz = vec3(0.0f);
    G_output[SPECULAR].w = 0.0f;

    G_output[NORMAL] = vec4(normalize(normal_cameraspace), 1.0f);
}
