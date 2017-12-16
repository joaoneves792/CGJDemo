#version 330 core

#define MAX_LIGHTS 20

in vec3 position_worldspace;
in float z_clipspace;
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
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 emissive;
uniform float shininess;
uniform float transparency;

uniform float movement; // get movement for the environment map

/*Lights Properties*/
uniform vec3 lightPosition_worldspace[MAX_LIGHTS];
uniform int lightsEnabled[MAX_LIGHTS];
uniform vec3 lightColor[MAX_LIGHTS];
uniform vec4 lightCone[MAX_LIGHTS]; //xyz->direction w->cutoffCos
uniform vec4 lightAttenuation[MAX_LIGHTS]; //x->constant y->linear z->quadratic w->range


void main() {
	//Material properties
	vec3 matDiffuse = (texture(texture_sampler, texture_coord_from_vshader).rgb * diffuse.xyz);

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

	    float lightSpecular;

        /*Blinn term*/
        vec3 H = normalize(L+E);
        lightSpecular = pow(clamp(dot(H, N), 0, 1), shininess*4.0f);

	    /*Diffuse component*/
	    lightContribution += matDiffuse*lightColor[i].xyz*clamp(dot(N, L), 0, 1);
	    /*Specular component*/
        lightContribution += specular.rgb*lightSpecular;
        /*Attenuation*/
        lightContribution *= 1.0f/(lightAttenuation[i].x + lightAttenuation[i].y*distanceToLight +
                             lightAttenuation[i].z*distanceToLight*distanceToLight);

        color += lightContribution;
	}

    float alpha = texture(texture_sampler, texture_coord_from_vshader).a - transparency;

    if(shininess >= 65.0f){
	    N = normalize(normal_worldspace);
        E = normalize(eyeDirection_worldspace);
        vec3 R = reflect(E, N);
        R = R + vec3(0.0f, 0.0f, movement);
        color += texture(environment, R).rgb*(shininess/128.0f)*0.1f*(1/alpha);
    }

    out_color[0].rgb = color + ambient.rgb*matDiffuse;
    out_color[0].a = alpha;

    out_color[1].rgb = normal_cameraspace;
    out_color[1].a = z_clipspace;


}
