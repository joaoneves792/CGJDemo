#version 330 core

#define MAX_LIGHTS 15

in vec2 uv;
in vec3 frustumRay;

out vec4 out_color;

/*Scene Properties*/
uniform mat4 View;
uniform mat4 Projection;

/*Material Properties*/
uniform sampler2D diffuse;
uniform sampler2D normals;
uniform sampler2D specular;
uniform sampler2D depth;

/*Lights Properties*/
uniform vec3 lightPosition_viewspace[MAX_LIGHTS];
uniform int lightsEnabled[MAX_LIGHTS];
uniform vec3 lightColor[MAX_LIGHTS];
uniform vec4 lightCone_viewspace[MAX_LIGHTS]; //xyz->direction w->cutoffCos
uniform vec4 lightAttenuation[MAX_LIGHTS]; //x->constant y->linear z->quadratic w->range

vec3 toneMappedTexture(sampler2D tex, vec2 uv){
    const float gamma = 2.2;
    return pow(texture(tex, uv).rgb, vec3(gamma));
}


void main() {
    //Reconstruct the position from depth and view ray
    float d = texture(depth, uv).r * 2.0f - 1.0f;
    float z = -(Projection[3][2]/(d+Projection[2][2]));
    vec3 position_viewspace = frustumRay*z;

	vec3 matDiffuse = toneMappedTexture(diffuse, uv);

	vec3 color = vec3(0,0,0);//start with black;
	vec3 N = normalize(texture(normals, uv).xyz);
	vec3 E = normalize(-position_viewspace);

    /*Go through the the lights until we hit a disabled one or MAX_LIGHTS (they should be sorted)*/
	for(int i=0; (bool(lightsEnabled[i]) && i<MAX_LIGHTS); i++){
		vec3 lightDirection_viewspace = lightPosition_viewspace[i] - position_viewspace;

	    float distanceToLight = length(lightDirection_viewspace);
	    if(distanceToLight > lightAttenuation[i].w && lightAttenuation[i].w > 0.0f){
	        continue;
	    }

		float cosCutOff = dot( normalize(lightCone_viewspace[i].xyz), normalize(-lightDirection_viewspace) );
		if(cosCutOff < lightCone_viewspace[i].w && lightCone_viewspace[i].w > -1.0f){
            continue;
        }
	    vec3 lightContribution = vec3(0,0,0);

	    vec3 L = normalize(lightDirection_viewspace);

        //Blinn term
        vec3 H = normalize(L+E);
        float shininess = texture(specular, uv).w*128.0;
        float lightSpecular = pow(clamp(dot(H, N), 0, 1), shininess*4.0f);

	    //Diffuse component
	    lightContribution += matDiffuse*lightColor[i].xyz*clamp(dot(N, L), 0, 1);
	    //Specular component
	    vec3 spec = texture(specular, uv).rgb;
        lightContribution += spec*lightSpecular;
        //Attenuation
        lightContribution *= 1.0f/(lightAttenuation[i].x + lightAttenuation[i].y*distanceToLight +
                             lightAttenuation[i].z*distanceToLight*distanceToLight);

        color += lightContribution;
	}

    //Check for NaN (caused by points infinitelly far away like the skybox)
    if( color.r >= 0){
        out_color.rgb = color;
    }else{
        out_color.rgb = vec3(0.0f);
    }
    out_color.a = 1.0f;

}
