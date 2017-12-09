#version 330 core

in vec3 vertex;
in vec4 state;

out vec2 position_modelspace;
out vec3 eyeDirection_worldspace;

uniform mat4 View;
uniform mat4 Projection;


void main() {
    vec3 position = state.xyz;
    float life = state.w;

    /*Billboarding*/
    mat3 rotMat = mat3(View);
    vec3 d = vec3(View[3]);

    vec3 eyePos = -d * rotMat;
    vec3 toEye = normalize(eyePos - position);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    vec3 right = normalize(cross(toEye, up));
    up = normalize(cross(toEye, right));


    /*Vertex generation*/
    float scale = clamp(2.0f + 0.5f * log(life), 0, 2);
    vec3 vPosition = (right*scale*vertex.x)+(up*scale*vertex.y)-(toEye*0.01f);
    vec4 clip_position = Projection * View * vec4(vPosition+position, 1.0f);

    /*Eye direction in worldspace*/
    eyeDirection_worldspace = normalize((vPosition+position) - eyePos);

  	gl_Position = clip_position;

	position_modelspace = vertex.xy;
}
