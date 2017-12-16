#version 330 core

#define KERNEL_SIZE 4*4
#define SAMPLE_RADIUS 1.0f

#define CAP_MIN_DISTANCE 0.0001
#define CAP_MAX_DISTANCE 0.005

in vec2 uv;
in vec3 frustumRay; //gets interpolated to a viewRay

uniform sampler2D colorBuffer;
uniform sampler2D depthBuffer;
uniform sampler2D noise;
uniform sampler2D normals;
uniform vec3 kernel[KERNEL_SIZE]; //4x4 kernel with a vec3 position
uniform mat4 P;
uniform mat4 inverseP;

out vec4 color;

void main() {
    float d = texture(depthBuffer, uv).r;

    /*Render depth buffer*/
    /*float n = 0.1;                                // the near plane
    float f = 100.0;                               // the far plane (should be 1000) in the CGJDemo
    float c = (2.0 * n) / (f + n - d * (f - n));  // convert to linear values
    vec3 depthColor = vec3(c);*/

    //float z = -(d*P[3][3]-P[3][2])/(d*P[2][3]-P[2][2]);
    float z = -(P[3][2]/(d+P[2][2]));

    vec3 origin = frustumRay*z;

    vec3 randomVector = normalize(vec3(texture(noise, uv*256.0f).xy, 0.0f) *2.0f - 1.0f);
    vec3 normalView = normalize(texture(normals, uv).xyz);
    vec3 tangentView = normalize(randomVector - dot(randomVector, normalView) * normalView);

	vec3 bitangentView = cross(normalView, tangentView);

	// Final matrix to reorient the kernel depending on the normal and the random vector.
    mat3 kernelMatrix = mat3(tangentView, bitangentView, normalView);


    float occlusion = 0.0;
    for (int i = 0; i < KERNEL_SIZE; ++i) {
        // get sample position:
        vec3 ksample = kernelMatrix * kernel[i];
        ksample = ksample * SAMPLE_RADIUS + origin.xyz;

        // project sample position:
        vec4 sample_pos = vec4(ksample.xyz, 1.0f);
        sample_pos = P * sample_pos;

        sample_pos.xyz /= sample_pos.w;
        sample_pos.xy = (sample_pos.xy+1)*0.5f;

        // get sample depth:
        float sampleDepth = texture(depthBuffer, sample_pos.xy).r;

        // range check & accumulate:
        //float rangeCheck= abs(sample_pos.z - sampleDepth) < SAMPLE_RADIUS ? 1.0 : 0.0;
        //occlusion += (sampleDepth >= sample_pos.z ? 1.0 : 0.0) * rangeCheck;

        float delta = sample_pos.z - sampleDepth;

        if (delta > CAP_MIN_DISTANCE && delta < CAP_MAX_DISTANCE){
        			occlusion += 1.0;
        }
    }


    //color.rgb = texture(colorBuffer, uv).rgb+(1-depthColor);
    color.rgb = vec3(1.0f-(occlusion/16.0f));
    //color.rgb = abs(origin);
    //color.rgb = normalize(ray.xyz);

    color.a = 1.0f;
}
