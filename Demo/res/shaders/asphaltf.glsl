#version 330 core

#define MAX_LIGHTS 20

in vec3 position_worldspace;
in vec3 eyeDirection_cameraspace;
in vec3 eyeDirection_worldspace;
in vec2 texture_coord_from_vshader;
in vec3 normal_cameraspace;
in vec3 normal_worldspace;
in vec3 lightDirection_cameraspace[MAX_LIGHTS];

out vec4[2] out_color;

/*Material Properties*/
uniform sampler2D texture_sampler;
uniform samplerCube environment;

uniform float night;

/*Lights Properties*/
uniform vec3 lightPosition_worldspace[MAX_LIGHTS];
uniform int lightsEnabled[MAX_LIGHTS];
uniform vec3 lightColor[MAX_LIGHTS];
uniform vec4 lightCone[MAX_LIGHTS]; //xyz->direction w->cutoffCos
uniform vec4 lightAttenuation[MAX_LIGHTS]; //x->constant y->linear z->quadratic w->range


void main() {
	vec3 matDiffuse = (texture(texture_sampler, texture_coord_from_vshader).rgb);

	vec3 color = vec3(0,0,0);//start with black;
	vec3 N = normalize(normal_cameraspace);
	vec3 E = normalize(eyeDirection_cameraspace);

	for(int i=0; i<MAX_LIGHTS; i++){
	    if(lightsEnabled[i] == 0){
	        continue;
	    }
	    float distanceToLight = length(lightPosition_worldspace[i] - position_worldspace);
	    if(distanceToLight > lightAttenuation[i].w && lightAttenuation[i].w > 0.0f){
	        continue;
	    }

		vec3 lightDirection_worldspace = lightPosition_worldspace[i].xyz - position_worldspace;
		float cosCutOff = dot( normalize(lightCone[i].xyz), normalize(-lightDirection_worldspace) );
		if(cosCutOff < lightCone[i].w && lightCone[i].w > -1.0f){
            continue;
        }
	    vec3 lightContribution = vec3(0,0,0);

	    vec3 L = normalize(lightDirection_cameraspace[i]);

	    /*Diffuse component*/
	    lightContribution += matDiffuse*lightColor[i].xyz*clamp(dot(N, L), 0, 1);
        /*Attenuation*/
        lightContribution *= 1.0f/(lightAttenuation[i].x + lightAttenuation[i].y*distanceToLight +
                             lightAttenuation[i].z*distanceToLight*distanceToLight);

        color += lightContribution;
	}

    if(night < 1.0f){
        N = normalize(normal_worldspace);
        E = normalize(eyeDirection_worldspace);
        vec3 R = reflect(E, N);
        float distanceCoef = clamp(abs(position_worldspace.z)/600.0f ,0.0f, 1.0f);
        distanceCoef *= smoothstep(0.1, 0.5, distanceCoef);
        vec3 ambient = texture(environment, R).rgb*distanceCoef;

        color += ambient.rgb+matDiffuse*0.1f;
    }else{
        color += matDiffuse*0.1;
    }

    out_color[0] = vec4(color, 1.0f);
    out_color[1] = vec4(normal_cameraspace, 1.0f);


}
