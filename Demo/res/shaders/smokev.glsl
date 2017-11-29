#version 330 core
in vec3 vertex;
in vec4 state;

out vec2 uv;
out float life;

uniform mat4 View;
uniform mat4 Projection;

void main() {
    vec3 position = state.xyz;
    life = state.w;

    mat3 rotMat = mat3(View);
    vec3 d = vec3(View[3]);

    vec3 eyePos = -d * rotMat;
    vec3 toEye = normalize(eyePos - position);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    vec3 right = cross(toEye, up);
    up = cross(toEye, right);

    vec3 vPosition = (right*life*vertex.x)+(up*life*vertex.y);

    vPosition += position;

    uv = ((vertex.xy*2.0f)+1.0f)/2.0f;
    gl_Position = Projection * View * vec4(vPosition, 1.0f);
}
