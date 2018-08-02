#version 330 core

in vec2 uv;
in vec3 frustumRay;

out vec4 out_color;

uniform sampler2D frame;
uniform sampler2D ambient;
uniform sampler2D ambientOcclusion;
uniform sampler2D depth;
uniform sampler2D shadow;

uniform mat4 depthBiasMVP;
uniform mat4 Projection;

#define ITERATIONS 16
#define CONTRIBUTION (((1.0f)/(ITERATIONS))/2.0f)

vec2 poissonDisk[16] = vec2[](
   vec2( -0.94201624, -0.39906216 ),
   vec2( 0.94558609, -0.76890725 ),
   vec2( -0.094184101, -0.92938870 ),
   vec2( 0.34495938, 0.29387760 ),
   vec2( -0.91588581, 0.45771432 ),
   vec2( -0.81544232, -0.87912464 ),
   vec2( -0.38277543, 0.27676845 ),
   vec2( 0.97484398, 0.75648379 ),
   vec2( 0.44323325, -0.97511554 ),
   vec2( 0.53742981, -0.47373420 ),
   vec2( -0.26496911, -0.41893023 ),
   vec2( 0.79197514, 0.19090188 ),
   vec2( -0.24188840, 0.99706507 ),
   vec2( -0.81409955, 0.91437590 ),
   vec2( 0.19984126, 0.78641367 ),
   vec2( 0.14383161, -0.14100790 )
);

void main() {
    //Reconstruct the position from depth and view ray
    float d = texture(depth, uv).r * 2.0f - 1.0f;
    float z = -(Projection[3][2]/(d+Projection[2][2]));
    vec3 position_viewspace = frustumRay*z;
    vec4 ShadowCoord = depthBiasMVP * vec4(position_viewspace, 1.0f);

    vec3 color = texture(frame, uv).rgb;
    vec3 ambientColor = texture(ambient, uv).rgb;
    float occlusion = texture(ambientOcclusion, uv).r;

    float visibility = 1.0f;
    /*float bias = 0.01;
    for (int i=0; i<16; i++){
        float contribution = 0.0625*(texture(shadow, ShadowCoord.xy + poissonDisk[i]/4000).r); //Grab color at point
        //Check depth
        visibility -= contribution * step(texture(shadow, ShadowCoord.xy).r, ((ShadowCoord.z-bias)/ ShadowCoord.w));
    }*/
    out_color.rgb = ((color*visibility + ambientColor)*occlusion);
    //out_color.rgb = vec3(visibility);
    //out_color.rgb = vec3((ShadowCoord.z-bias)/ShadowCoord.w);
    //out_color.rgb  = vec3(texture( shadow, vec3(ShadowCoord.xy, (ShadowCoord.z-bias)/ ShadowCoord.w) ));
    //out_color.rgb  = texture(shadow, ShadowCoord.xy).rrr;
    //out_color.rgb = position_viewspace;
    out_color.rgb = ((ambientColor));
    out_color.a = 1.0f;


    /*float visibility = 1.0f;
	float bias = 0.001;
	ShadowCoord.xy = clamp(ShadowCoord.xy, 0.01f, 0.99f);
    for (int i=0; i<ITERATIONS; i++){
        float qSample = texture(shadow, ShadowCoord.xy + poissonDisk[i]/1000.0f).r;
  		float contribution = CONTRIBUTION;//*(1-abs(qInShadow-qSample));
    	//Check depth
        visibility -= contribution * step(qSample, qInShadow-bias); //Only contribute if qSample < qInShadow
	}*/

    //out_color.rgb = (color + ambientColor)*occlusion;

}
