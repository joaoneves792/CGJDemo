#version 330 core

in vec3 vertex;

out vec2 uv;
out vec3 eyeDirection_worldspace;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;


void main() {
    vec4 position_worldspace = Model*vec4(vertex, 1.0f);
    mat3 rotMat = mat3(View);
    vec3 d = vec3(View[3]);
    vec3 eyePos = -d * rotMat;

    /*Eye direction in worldspace*/
    eyeDirection_worldspace = position_worldspace.xyz - eyePos;

  	gl_Position = Projection * View * position_worldspace;

	uv.x = (vertex.x+1)/2;
	uv.y = (vertex.y+1)/2;
}
