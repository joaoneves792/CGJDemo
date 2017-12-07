#version 330 core

in vec2 uv;
in vec2 position_modelspace;
in float life;

uniform sampler2D renderedTexture;
uniform float black;

out vec4 color;

#define PI 3.14159f
#define AMPLITUDE 1.0f/500.0f
#define FREQUENCY 2.0f*PI
#define PHASE 100.0f

#define X_AMPLITUDE 0.8f
#define Y_AMPLITUDE 0.8f

#define HARMONICS_A 1.2f
#define HARMONICS_B 1.5f
#define HARMONICS_N 6

void main() {
    vec2 texcoord = uv;
    for(int i=0; i<HARMONICS_N; i++){
        texcoord.x += (X_AMPLITUDE*AMPLITUDE*sin(pow(HARMONICS_B, i)*FREQUENCY + life*PHASE))/pow(HARMONICS_A, i);
        texcoord.y += (Y_AMPLITUDE*AMPLITUDE*sin(pow(HARMONICS_B, i)*FREQUENCY + life*PHASE))/pow(HARMONICS_A, i);
    }
    color.rgb = texture(renderedTexture, texcoord).rgb;
    color.a = 1.0f-length(position_modelspace)*2;

    //color = vec4(texture(renderedTexture, (position_modelspace+1)/2).rgb, 1.0f);
    if(black > 0.0){
        color.rgb = vec3(0.0f, 0.0f, 0.0f);
    }
}
