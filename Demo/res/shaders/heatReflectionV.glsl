#version 330 core

in vec3 vertex;

out vec2 mirror_uv;
out vec2 standard_uv;
out vec3 eyeDirection_worldspace;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 reflectionView;


void main() {
    vec4 position_worldspace = Model*vec4(vertex, 1.0f);
    mat3 rotMat = mat3(View);
    vec3 d = vec3(View[3]);
    vec3 eyePos = -d * rotMat;

    /*Eye direction in worldspace*/
    eyeDirection_worldspace = position_worldspace.xyz - eyePos;

  	gl_Position = Projection * View * position_worldspace;

    vec4 vClipReflection = Projection * reflectionView * Model * vec4(vertex.xy, 0.0f , 1.0f);
	vec2 vDeviceReflection = vClipReflection.st / vClipReflection.q;
	mirror_uv = vec2(0.5, 0.5f) + 0.5f * vDeviceReflection;

	standard_uv = (vertex.xy+1.0f)/2.0f;
}
