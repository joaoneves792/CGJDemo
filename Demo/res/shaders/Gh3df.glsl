#version 330 core

#define DIFFUSE 0
#define AMBIENT 1
#define SPECULAR 2
#define NORMAL 3

in vec3 eyeDirection_worldspace;
in vec2 texture_coord_from_vshader;
in vec3 normal_cameraspace;
in vec3 normal_worldspace;

out vec4[4] G_output;

/*Material Properties*/
uniform sampler2D texture_sampler;
uniform samplerCube environment;
uniform float ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 emissive;
uniform float shininess;
uniform float transparency;

uniform float movement; // get movement for the environment map

vec3 fresnelSchlick(float cosTheta, vec3 F0){
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main() {
	//Material properties
	vec3 matDiffuse = (texture(texture_sampler, texture_coord_from_vshader).rgb * diffuse.xyz);

    //For fully correct shading, fragments with alpha < 1.0f should only be drawn in a second separate pass
    //We are however drawing everything at once, so some visual glitches may appear
    float alpha = texture(texture_sampler, texture_coord_from_vshader).a - transparency;

    if(shininess >= 65.0f){
	    vec3 N = normalize(normal_worldspace);
        vec3 E = normalize(eyeDirection_worldspace);
        vec3 R = reflect(E, N);
        R = R + vec3(0.0f, 0.0f, movement);
        float NdotV = max(dot(N, E), 0.0f);
        vec3 fresnel = fresnelSchlick(NdotV, vec3(0.96f, 0.96f, 0.97f));
        matDiffuse += (texture(environment, R).rgb*(shininess/128.0f)*0.35f)*fresnel;//*(1/alpha);
    }

    if(alpha == 0.0f){
        discard;
    }
    G_output[DIFFUSE].rgb = matDiffuse;
    G_output[DIFFUSE].a = alpha;

    G_output[AMBIENT].rgb = ambient*3.5f*matDiffuse;
    G_output[AMBIENT].a = 1.0f;

    G_output[SPECULAR].xyz = specular.rgb;
    G_output[SPECULAR].w = shininess;

    G_output[NORMAL] = vec4(normal_cameraspace, 1.0f);
}
