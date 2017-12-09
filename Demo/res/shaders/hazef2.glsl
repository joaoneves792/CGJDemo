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

#define X_AMPLITUDE 0.8f
#define Y_AMPLITUDE 0.8f

#define HARMONICS_A 1.2f
#define HARMONICS_B 1.5f
#define HARMONICS_N 6

float lines(float y, float b){
    y *= 10.0f;
    //return smoothstep(0.0, 0.5+b*.5, (sin(pos.y*3.1415)+b*2.0));
    return clamp(0, 1, (sin(y*3.1415)+b));
}

float harmonic_sin(int i, float a, float f){
         return (AMPLITUDE*sin(pow(HARMONICS_B, i)*FREQUENCY*a + f*PHASE))/pow(HARMONICS_A, i);
}

void main() {
	vec2 pos = position_modelspace;
    vec2 noise = vec2(0.0f, 0.0f);
    for(int i=0; i<HARMONICS_N; i++){
        float hs = harmonic_sin(i, pos.x, life);
        noise.x += X_AMPLITUDE*hs;
        noise.y += Y_AMPLITUDE*hs;
    }
    color.rgb = texture(renderedTexture, uv+noise).rgb;
    color.a = 1.0f-length(position_modelspace)*2;

    //color = vec4(texture(renderedTexture, (position_modelspace+1)/2).rgb, 1.0f);
    if(black > 0.0){
        float y = 0.0f;
        /*for(int i=0; i<4; i++){
            //y += harmonic_sin(i, pos.x, life);
            y += (0.1*sin(pow(HARMONICS_B, i)*2*PI*pos.x+life*PHASE)/pow(HARMONICS_A, i));
        }*/
        y= 40.0f*noise.x;
	    /*float y = (0.1*
	                (sin(10*pos.x*3.14159f))
	              ) - 0.5 + life;*/
        if(pos.y < y + 0.01 && pos.y > y-0.01)
            color.rgb = vec3(1.0f, 1.0f, 1.0f);
        else
            color.rgb = vec3(0.0f, 0.0f, 0.0f);
    }
}
