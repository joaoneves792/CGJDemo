#version 330 core

#define DIFFUSE 0
#define AMBIENT 1
#define SPECULAR 2
#define NORMAL 3

in vec3 eyeDirection_worldspace;
in vec2 texture_coord_from_vshader;
in vec3 normal_cameraspace;
in vec3 normal_worldspace;
in vec3 position_worldspace;

out vec4[5] G_output;

/*Material Properties*/
uniform sampler2D texture_sampler;
uniform samplerCube environment;

uniform float night;

void main() {
	//Material properties
	vec3 matDiffuse = (texture(texture_sampler, texture_coord_from_vshader).rgb);
    vec3 reflection = vec3(0.0f);

    if(night < 1.0f){
        vec3 N = normalize(normal_worldspace);
        vec3 E = normalize(eyeDirection_worldspace);
        vec3 R = reflect(E, N);
        float distanceCoef = clamp(abs(position_worldspace.z)/600.0f ,0.0f, 1.0f);
        distanceCoef *= smoothstep(0.1, 0.5, distanceCoef);
        vec3 ambient = texture(environment, R).rgb*distanceCoef;

        reflection += ambient.rgb+matDiffuse*0.1f;
    }else{
        reflection += matDiffuse*0.1;
    }

    G_output[DIFFUSE].rgb = matDiffuse;
    G_output[DIFFUSE].a = 1.0f;

    G_output[AMBIENT].rgb = reflection;
    G_output[AMBIENT].a = 1.0f;

    G_output[SPECULAR] = vec4(0.0f);

    G_output[NORMAL] = vec4(normal_cameraspace, 1.0f);
}
