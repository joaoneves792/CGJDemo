#version 330 core

in vec3 vertex;
in vec4 state;

out vec2 uv;
out vec2 position_modelspace;
out float life;
out vec4 position;

uniform mat4 View;
uniform mat4 Projection;


void main() {
    vec3 pos = state.xyz;
    life = state.w;

    /*Billboarding*/
    mat3 rotMat = mat3(View);
    vec3 d = vec3(View[3]);

    vec3 eyePos = -d * rotMat;
    vec3 toEye = normalize(eyePos - pos);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    vec3 right = normalize(cross(toEye, up));
    up = normalize(cross(toEye, right));

    /*Vertex generation*/
    float scale = 3.0f;// clamp(3.0f + 0.5f * log(life), 0, 2);
    vec3 vPosition = (right*scale*vertex.x)+(up*scale*vertex.y);
    vec4 pos_viewspace = View * vec4(vPosition+pos, 1.0f);
    vec4 clip_position = Projection * pos_viewspace;

    /*Texcoords generation*/
	//uv = vertex.xy*0.5+0.5;
    uv = ((vertex.xy*2.0f)+1.0f)/2.0f;


  	gl_Position = clip_position;

  	position = vec4(clip_position.xyz / clip_position.w, pos_viewspace.z);
	position.xy = position.xy*0.5+0.5;


	position_modelspace = vertex.xy;
}
