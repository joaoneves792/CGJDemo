#version 330 core

in vec3 inPosition;

out vec3 eyeDirection_worldspace;
out vec3 position_worldspace;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 MVP;

void main() {
    vec4 position = vec4(inPosition, 1.0f);

	/*Position in worldspace and cameraspace*/
	position_worldspace = (Model * position).xyz;

    /*Eye direction in worldspace*/
    mat4 MV = View;
    mat3 rotMat = mat3(MV);
    vec3 d = vec3(MV[3]);
    vec3 eyePos = -d * rotMat;
    eyeDirection_worldspace = position_worldspace - eyePos;

    vec4 position_clipspace = MVP * position;
	gl_Position = position_clipspace;
}
