#version 330 core

in vec3 fragmentPosition;
in vec3 position_worldspace;
in vec3 normal_cameraspace;
in vec3 eyeDirection_cameraspace;
in vec3 lightDirection_cameraspace;
in float transparency_from_vshader;

out vec4 outColor;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(void){
    vec3 baseColor = vec3(0.61568, 0.33725, 0.0);
    //vec3 baseColor = vec3(157, 86, 0.0);

    vec4 lightColor = vec4(0.1, 0.1, 0.1, 5); // x,y,z -> color rgb; w -> intensity (if < 0 then does not decay)
    vec3 lightCone = vec3(0.3, -1, 0); //x,y,z -> direction; w -> cutoffanglecos (if < 0 then emmits in all directions)
    vec3 lightPosition_worldspace = vec3(5, 20, 5);

	vec3 n = normalize( normal_cameraspace );
	vec3 E = normalize(eyeDirection_cameraspace);

	vec3 lightDirection_worldspace = lightPosition_worldspace.xyz - position_worldspace;
	float distance_to_light = length( lightDirection_worldspace );

	vec3 l = normalize( lightDirection_cameraspace );

	vec3 R = reflect(-l, n);
	float cosAlpha = clamp( dot(E,R), 0.0,1.0);
    float cosTheta = clamp( dot(n,l), 0.0,1.0);

	baseColor.rgb = baseColor.rgb * rand(floor(200*fragmentPosition.xz));
	baseColor.r = clamp(baseColor.r, 0.2f, baseColor.r);
	baseColor.g = clamp(baseColor.r, 0.2f, baseColor.g);



	outColor.rgb = baseColor * lightColor.xyz * abs(lightColor.w) * cosTheta +
		lightColor.xyz * abs(lightColor.w) * pow(cosAlpha, 5.0);
    //outColor.rgb = baseColor;
    outColor.a = 1.0f;
}

