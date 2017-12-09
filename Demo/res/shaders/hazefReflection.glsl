#version 330 core

in vec2 position_modelspace;
in vec3 eyeDirection_worldspace;

uniform samplerCube environment;

out vec4 color;

void main() {
	vec3 N = vec3(0.0f, 1.0f, 0.0f);
    vec3 E = normalize(eyeDirection_worldspace);
    vec3 R = reflect(E, N);
    color.rgb = texture(environment, R).rgb;
    color.a = 1.0f-length(position_modelspace)*2;
}
