#version 330 core

in vec2 uv;
in vec3 frustumRay;

out vec4 out_color;

uniform sampler2D frame;
uniform sampler2D ambient;
uniform sampler2D ambientOcclusion;
uniform sampler2D depth;
uniform sampler2DShadow shadow;

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



vec3 toneMappedTexture(sampler2D tex, vec2 uv){
    const float gamma = 2.2;
    return pow(texture(tex, uv).rgb, vec3(gamma));
}
vec3 toneMapping(vec3 color){
    const float gamma = 2.2;
    const float exposure = 1.1f;
    // Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-color * exposure);
    // Gamma correction
    return pow(mapped.rgb, vec3(1.0 / gamma));

}

void main() {
    //Reconstruct the position from depth and view ray
    float d = texture(depth, uv).r * 2.0f - 1.0f;
    float z = -(Projection[3][2]/(d+Projection[2][2]));
    vec3 position_viewspace = frustumRay*z;
    vec4 ShadowCoord = depthBiasMVP * vec4(position_viewspace, 1.0f);
    ShadowCoord.xy = ShadowCoord.xy/ShadowCoord.w;

    vec3 color = texture(frame, uv).rgb;
    vec3 ambientColor = toneMappedTexture(ambient, uv);
    float occlusion = texture(ambientOcclusion, uv).r;

    float visibility = 1.0f;
    float bias = 0.005;
    if ((ShadowCoord.x < 0.0f ||  ShadowCoord.x > 1.0f) && (ShadowCoord.y > 0.2f || ShadowCoord.y < 0.0f)){
        out_color.rgb = ((toneMapping(color) + ambientColor)*occlusion);
        out_color.a = 1.0f;
        return;
    }
    if(ShadowCoord.y < 0.0f || ShadowCoord.y > 1.0f){
        out_color.rgb = ((toneMapping(color) + ambientColor)*occlusion);
        out_color.a = 1.0f;
        return;
    }
    for (int i=0; i<16; i++){
   		visibility -= 0.05*(texture(shadow, vec3(ShadowCoord.xy + poissonDisk[i]/700, (ShadowCoord.z-bias)/ ShadowCoord.w) ) );
    }

    //ToneMapping just "color" is for artistic purposes, for correctness we should toneMap the whole thing!
    out_color.rgb = ((toneMapping(color*visibility) + ambientColor)*occlusion);
    //out_color.rgb = (ambientColor);
    out_color.a = 1.0f;

}
