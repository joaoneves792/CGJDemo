#version 330 core

in vec2 mirror_uv;
in vec2 standard_uv;
in vec3 eyeDirection_worldspace;

uniform sampler2D map;
uniform sampler2D mirror;
uniform float time;

out vec4 color;

#define PI 3.14159f
#define AMPLITUDE 0.2f
#define FREQUENCY PI
#define PHASE 0.0f

#define HARMONICS_A 1.2f
#define HARMONICS_B 2.0f

#define NEXT_TEXEL_STEP 0.0025f
#define BLUR_LINE_WIDTH 0.1f


float blur_kernel[9] = float[](0.05f, 0.15f, 0.05f,
                               0.15f, 0.15f, 0.15f,
                               0.05f, 0.15f, 0.05f);


vec3 convolute(float[9] kernel, vec2 pos){
    vec3 t[9] = vec3[](
        texture(mirror, vec2(pos.x-NEXT_TEXEL_STEP, pos.y+NEXT_TEXEL_STEP)).rgb,
        texture(mirror, vec2(pos.x, pos.y+NEXT_TEXEL_STEP)).rgb,
        texture(mirror, vec2(pos.x+NEXT_TEXEL_STEP, pos.y+NEXT_TEXEL_STEP)).rgb,

        texture(mirror, vec2(pos.x-NEXT_TEXEL_STEP, pos.y)).rgb,
        texture(mirror, vec2(pos.x, pos.y)).rgb,
        texture(mirror, vec2(pos.x+NEXT_TEXEL_STEP, pos.y)).rgb,

        texture(mirror, vec2(pos.x-NEXT_TEXEL_STEP, pos.y-NEXT_TEXEL_STEP)).rgb,
        texture(mirror, vec2(pos.x, pos.y-NEXT_TEXEL_STEP)).rgb,
        texture(mirror, vec2(pos.x+NEXT_TEXEL_STEP, pos.y-NEXT_TEXEL_STEP)).rgb
    );

    return kernel[8]*t[0] + kernel[7]*t[1] + kernel[6]*t[2] +
           kernel[5]*t[3] + kernel[4]*t[4] + kernel[3]*t[5] +
           kernel[2]*t[6] + kernel[1]*t[7] + kernel[0]*t[8];
}

float harmonic_sin(int i, float a, float f){
         return (AMPLITUDE*sin(pow(HARMONICS_B, i)*FREQUENCY*a + f*PHASE))/pow(HARMONICS_A, i);
}

void main() {
    float timeFactor = 0.0f;
    timeFactor += harmonic_sin(0, time, 0.0f);
    timeFactor += harmonic_sin(2, time, 0.0f);
    timeFactor += harmonic_sin(4, time, 0.0f);

    float coef = (1-abs(dot(normalize(eyeDirection_worldspace), vec3(0.0f, 1.0f, 0.0f))))*0.5f;

    vec2 map_uv = vec2(standard_uv.x, standard_uv.y - timeFactor*0.25f);
    float factor = texture(map, map_uv).r - abs(timeFactor)-0.1;
    color.rgb = convolute(blur_kernel, mirror_uv)*factor*0.5f;
    color.a = factor*coef;
}
