#version 330 core

in vec2 uv;
in float life;
in vec4 position;


uniform sampler2D tex;
uniform sampler2D depth;

uniform mat4 Projection;

out vec4 color;
#define CONTRAST_POWER 2.0f
#define SCALE 0.5f


/*Code borrowed from NVIDIA smooth particles example
* Available here: http://developer.download.nvidia.com/SDK/10/direct3d/samples.html
*/
float contrast(float Input, float ContrastPower)
{
/**/
     //piecewise contrast function
     bool IsAboveHalf = Input > 0.5 ;
     float ToRaise = clamp(2*(IsAboveHalf ? 1-Input : Input), 0.0f, 1.0f);
     float Output = 0.5*pow(ToRaise, ContrastPower);
     Output = IsAboveHalf ? 1-Output : Output;
     return Output;
/**/
/*
    // another solution to create a kind of contrast function
    return 1.0 - exp2(-2*pow(2.0*clamp(Input, 0.0f, 1.0f), ContrastPower));
*/
}


void main() {
    float d_scene = texture(depth, position.xy).r*2.0f-1.0f;
    if (position.z >= d_scene)
        discard;

    float z_scene = -(Projection[3][2]/(d_scene+Projection[2][2]));
    float z_delta = position.w - z_scene;

    float c = contrast(z_delta * SCALE, CONTRAST_POWER);

    //color.rgb = vec3(1.0f);
    color.rgba = texture(tex, uv).rgba;
    color.a *= c*life;
    /*color.rgb = vec3(abs(z_delta));
    color.a = 1.0f;*/
}
