#version 330 core

in vec3 vertex;
in vec4 state;

out vec2 uv;
out vec2 position_modelspace;
out float life;

uniform mat4 View;
uniform mat4 Projection;


void main() {
    vec3 position = state.xyz;
    life = state.w;

    /*Billboarding*/
    mat3 rotMat = mat3(View);
    vec3 d = vec3(View[3]);

    vec3 eyePos = -d * rotMat;
    vec3 toEye = normalize(eyePos - position);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    vec3 right = normalize(cross(toEye, up));
    up = normalize(cross(toEye, right));

    /*Vertex generation*/
    float scale = clamp(3.0f + 0.5f * log(life), 0, 2);
    vec3 vPosition = (right*scale*vertex.x)+(up*scale*vertex.y);
    vec4 clip_position = Projection * View * vec4(vPosition+position, 1.0f);

    /*Texcoords generation*/
	vec3 ndc = clip_position.xyz / clip_position.w;
	//uv = clamp(ndc.xy, -1.0f, 1.0f)*0.5+0.5;
	uv = ndc.xy*0.5+0.5;


  	gl_Position = clip_position;

	position_modelspace = vertex.xy;
}
