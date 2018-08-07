#version 330 core

#define DIFFUSE 0
#define AMBIENT 1
#define SPECULAR 2
#define NORMAL 3

in vec3 eyeDirection_worldspace;
in vec3 position_worldspace;

out vec4[4] G_output;

uniform mat4 View;
uniform mat4 Projection;

#define STEPS 64
#define STEP_SIZE 0.2
#define MIN_DISTANCE 0.2

float map(vec3 p){
    return distance(p, vec3(20.0f, 5.0f, -20.0f)) - 5.0f;
}

vec3 get_normal(vec3 p){
 const float eps = 0.01;
 return normalize( vec3( map(p + vec3(eps, 0, 0) ) - map(p - vec3(eps, 0, 0)),
                         map(p + vec3(0, eps, 0) ) - map(p - vec3(0, eps, 0)),
                         map(p + vec3(0, 0, eps) ) - map(p - vec3(0, 0, eps))
                        )
                 );
}
void main() {
    vec3 position = position_worldspace;
    vec3 direction = normalize(eyeDirection_worldspace);

    vec4 color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec4 normal = vec4(0.0f, 1.0f, 0.0f, 0.0f);

    for (int i = 0; i < STEPS; i++){
        float distance = map(position);
        if(distance < MIN_DISTANCE){
            color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
            normal = vec4(get_normal(position), 0.0f);
            G_output[DIFFUSE].rgba = color;
            G_output[AMBIENT].rgba = color;
            G_output[SPECULAR].xyz = vec3(1.0f, 1.0f, 1.0f);
            G_output[SPECULAR].w = 0.2f;
            G_output[NORMAL] = View*normal;
            float new_z = (View*vec4(position, 1.0f)).z;
            //float depth = 0.5*(Projection[2].z*new_z+Projection[3].z)/(-new_z)+0.5;
            float depth = (Projection[2].z*new_z+Projection[3].z)/(-new_z);
            gl_FragDepth = depth;
            return;
        }

        position += direction * distance;
    }

    discard;
}
