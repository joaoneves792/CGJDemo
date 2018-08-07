#version 330 core

in vec4 inPosition;

out vec3 position_worldspace;
out vec3 normal_cameraspace;

uniform mat4 MVP;
uniform mat4 Model;
uniform mat4 View;

void main() {
	/*Position in worldspace and cameraspace*/
	position_worldspace = (Model * inPosition).xyz;

    vec4 position_clipspace = MVP * inPosition;
	gl_Position = position_clipspace;

	normal_cameraspace = (View * vec4(0.0f, 1.0f, 0.0f, 0.0f)).xyz;
}
