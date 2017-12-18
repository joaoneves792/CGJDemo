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
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 emissive;
uniform float shininess;
uniform float transparency;

uniform float movement; // get movement for the environment map

void main() {
	//Material properties
	vec3 matDiffuse = (texture(texture_sampler, texture_coord_from_vshader).rgb * diffuse.xyz);

    float alpha = texture(texture_sampler, texture_coord_from_vshader).a - transparency;

    if(shininess >= 65.0f){
	    vec3 N = normalize(normal_worldspace);
        vec3 E = normalize(eyeDirection_worldspace);
        vec3 R = reflect(E, N);
        R = R + vec3(0.0f, 0.0f, movement);
        matDiffuse += texture(environment, R).rgb*(shininess/128.0f)*0.3f*(1/alpha);
    }

    G_output[DIFFUSE].rgb = matDiffuse;
    G_output[DIFFUSE].a = alpha;

    G_output[AMBIENT] = ambient*vec4(matDiffuse, 1.0f);

    G_output[SPECULAR].xyz = specular.rgb;
    G_output[SPECULAR].w = shininess;

    G_output[NORMAL] = vec4(normal_cameraspace, 1.0f);
}
