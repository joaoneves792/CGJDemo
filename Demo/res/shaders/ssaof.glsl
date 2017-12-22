#version 330 core

/* SSAO implementation
 * based on examples provided in:
 * https://john-chapman-graphics.blogspot.pt/2013/01/ssao-tutorial.html
 * https://github.com/McNopper/OpenGL/blob/master/Example28/shader/ssao.frag.glsl
 * https://mynameismjp.wordpress.com/2009/03/10/reconstructing-position-from-depth/
 * http://www.derschmale.com/2014/01/26/reconstructing-positions-from-the-depth-buffer/
 */


#define KERNEL_SIZE 4*4
#define SAMPLE_RADIUS 1.5f

#define CAP_MIN_DISTANCE 0.00001
#define CAP_MAX_DISTANCE 1.0f

in vec2 uv;
in vec3 frustumRay; //gets interpolated to a viewRay

uniform sampler2D depthBuffer;
uniform sampler2D noise;
uniform sampler2D normals;
//uniform vec3 kernel[KERNEL_SIZE]; //4x4 kernel with a vec3 position
uniform mat4 P;

out vec4 color;

//Random kernel
vec3 kernel[KERNEL_SIZE] = vec3[](
    vec3(0.0836703f, 0.042399f, 0.0346641f),
    vec3(0.0488528f, 0.0652283f, 0.0638292f),
    vec3(0.0672022f, 0.0792735f, 0.0470088f),
    vec3(-0.0345514f, -0.0627393f, 0.11045f),
    vec3(-0.135339f, 0.000644491f, 0.0780834f),
    vec3(0.153842f, -0.0509055f, 0.0951012f),
    vec3(-0.173628f, 0.0876701f, 0.11618f),
    vec3(0.0316623f, 0.216534f, 0.161985f),
    vec3(-0.200245f, -0.0925606f, 0.238662f),
    vec3(-0.269237f, -0.0414442f, 0.271732f),
    vec3(0.339132f, 0.237846f, 0.179798f),
    vec3(0.317791f, 0.396569f, 0.133331f),
    vec3(0.309548f, -0.512734f, 0.0939292f),
    vec3(-0.602026f, -0.143371f, 0.31439f),
    vec3(0.225538f, 0.541993f, 0.527253f),
    vec3(-0.683553f, -0.164809f, 0.547267f)
);

void main() {
    //Reconstruct the position from depth and view ray
    float d = texture(depthBuffer, uv).r*2.0f-1.0f;
    //float z = -(d*P[3][3]-P[3][2])/(d*P[2][3]-P[2][2]); //Can be simplified to:
    float z = -(P[3][2]/(d+P[2][2]));
    vec3 origin = frustumRay*z;

    //Fetch the normal and random vector
    vec3 normalView = normalize(texture(normals, uv).xyz);
    vec2 noiseScaleFactor = textureSize(depthBuffer, 0)/textureSize(noise, 0);
    vec3 rvec = vec3(texture(noise, uv * noiseScaleFactor).xy, 0.0f) * 2.0 - 1.0;

    //Gram–Schmidt process
    vec3 tangent = normalize(rvec - normalView * dot(rvec, normalView));
    vec3 bitangent = cross(normalView, tangent);
    mat3 tbn = mat3(tangent, bitangent, normalView);

    float occlusion = 0.0;
    for (int i = 0; i < KERNEL_SIZE; ++i) {
        // get sample position:
        vec3 ksample = tbn * kernel[i];
        ksample = ksample * SAMPLE_RADIUS + origin.xyz;

        // project sample position:
        vec4 sample_pos = vec4(ksample.xyz, 1.0f);
        sample_pos = P * sample_pos;

        sample_pos.xyz /= sample_pos.w;
        sample_pos.xy = (sample_pos.xy+1)*0.5f;

        if(sample_pos.x > 1.0f || sample_pos.x < 0.0f || sample_pos.y > 1.0f || sample_pos.y < 0.0f){
            continue; //Sample is outside the screen if we sample its depth we're getting wrong values
        }

        //sample the depth buffer:
        float sampleDepth = texture(depthBuffer, sample_pos.xy).r*2.0f-1.0f;

        //Compare samples depth with depth of scene
        float delta = sample_pos.z - sampleDepth;


        if (delta > CAP_MIN_DISTANCE && delta < CAP_MAX_DISTANCE){
            occlusion += 1.0;
        }
    }


    color.rgb = vec3(1.0f-(occlusion/float(KERNEL_SIZE)));

    color.a = 1.0f;
}
