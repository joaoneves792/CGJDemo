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

    /*Billboarding*/
    mat3 rotMat = mat3(View);
    vec3 d = vec3(View[3]);

    vec3 eyePos = -d * rotMat;
    vec3 toEye = normalize(eyePos - position);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    vec3 right = normalize(cross(toEye, up));
    up = normalize(cross(toEye, right));


    /*Vertex generation*/
    float scale = 0.3/clamp(life, 0.3, 1.0);
    vec3 vPosition = (right*scale*vertex.x)+(up*scale*vertex.y);
    vPosition += position;
    gl_Position = Projection * View * vec4(vPosition, 1.0f);

    /*Texcoords generation*/
    uv = ((vertex.xy*2.0f)+1.0f)/2.0f;
    vec2 tempUV = uv;
    float step = 1.0f/8.0f;
    float steps = 64;
    int currentStep = int(floor(life*steps));
    uv.x = (currentStep % 8)*step + step*tempUV.x;
    uv.y = 1.0f - (floor(currentStep/8.0f)*step + step*tempUV.y);

}
