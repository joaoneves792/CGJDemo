#version 330 core

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;
in vec3 inJointIndex;
in vec3 inJointWeight;
in vec3 inTangent;
in vec3 inBitangent;

out vec3 eyeDirection_worldspace;
out vec2 texture_coord_from_vshader;
out vec3 normal_cameraspace;
out vec3 tangent_cameraspace;
out vec3 bitangent_cameraspace;
out vec3 normal_worldspace;
out vec3 position_worldspace;
out vec3 position_cameraspace;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 NormalMatrix;
uniform mat4 MVP;

void main() {
    vec4 position = vec4(inPosition, 1.0f);

    /*Calculate normals*/
    normal_cameraspace = ((NormalMatrix *  vec4(inNormal, 0.0f)).xyz);
    tangent_cameraspace = ((NormalMatrix *  vec4(inTangent, 0.0f)).xyz);
    bitangent_cameraspace = ((NormalMatrix * vec4(inBitangent, 0.0f)).xyz);
    normal_worldspace = (Model * vec4(inNormal, 0.0f)).xyz; //Assuming there is no scale

	/*Position in worldspace and cameraspace*/
	position_worldspace = (Model * position).xyz;
    //position_cameraspace = (View * vec4(position_worldspace, 1.0f)).xyz;
    position_cameraspace = (View * vec4(position_worldspace, 1.0f)).xyz;

    /*Eye direction in worldspace*/
    mat4 MV = View;
    mat3 rotMat = mat3(MV);
    vec3 d = vec3(MV[3]);
    vec3 eyePos = -d * rotMat;
    eyeDirection_worldspace = position_worldspace - eyePos;

    vec4 position_clipspace = MVP * position;
	gl_Position = position_clipspace;

	texture_coord_from_vshader = inTexCoord;
}
