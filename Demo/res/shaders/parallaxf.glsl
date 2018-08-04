#version 330 core

#define DIFFUSE 0
#define AMBIENT 1
#define SPECULAR 2
#define NORMAL 3

in vec3 eyeDirection_worldspace;
in vec2 texture_coord_from_vshader;
in vec3 normal_cameraspace;
in vec3 tangent_cameraspace;
in vec3 bitangent_cameraspace;
in vec3 normal_worldspace;
in vec3 position_cameraspace;
in highp float depth;

out vec4[4] G_output;

/*Material Properties*/
uniform sampler2D diffuse;
uniform sampler2D normals;
uniform sampler2D bump;
//uniform samplerCube environment;
uniform mat4 Projection;

#define SCALE 0.0005
#define DEPTH_SCALE_BIAS 0.05

void main() {

    mat3 TBN = mat3(normalize(tangent_cameraspace),
                    normalize(bitangent_cameraspace),
                    normalize(normal_cameraspace));

    //Parallax mapping
    mat3 invTBN = transpose(TBN);
    vec3 camera_tangentspace = invTBN * normalize(-position_cameraspace);
	float h = texture(bump, texture_coord_from_vshader).r;
	//float displacement = h * SCALE - BIAS;
    vec2 displacement = camera_tangentspace.xy / camera_tangentspace.z * (h*SCALE);
    //vec2 displaced_uv = texture_coord_from_vshader - displacement;
    vec2 displaced_uv = texture_coord_from_vshader + displacement;

    vec3 normal = (2.0f*texture(normals, displaced_uv).rgb)-1.0f;
	vec3 matDiffuse = (texture(diffuse, displaced_uv).rgb);
    vec3 new_normal = TBN*normal;


    //vec3 offset = normalize(new_normal)*h*DEPTH_SCALE_BIAS;
    //vec4 pos_clipspace = Projection * vec4(position_cameraspace + offset, 1.0f);
    //float depth = 0.5*(pos_clipspace.z/pos_clipspace.w) + 0.5; // + -> push - -> pull
    float new_z = position_cameraspace.z + normalize(new_normal).z*h*DEPTH_SCALE_BIAS;
    float depth = 0.5*(Projection[2].z*new_z+Projection[3].z)/(-new_z)+0.5;
    gl_FragDepth = depth;

    G_output[DIFFUSE].rgb = matDiffuse;
    G_output[DIFFUSE].a = 1.0f;

    //G_output[AMBIENT].rgb = vec3(normalize(new_normal));
    G_output[AMBIENT].rgb = matDiffuse*0.3f;
    G_output[AMBIENT].a = 1.0f;

    G_output[SPECULAR].xyz = vec3(0.0f);
    G_output[SPECULAR].w = 0.0f;

    G_output[NORMAL] = vec4(new_normal, 1.0f);
}
