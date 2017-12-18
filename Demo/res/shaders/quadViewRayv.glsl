#version 330 core

in vec3 inPosition;

out vec2 uv;
out vec3 frustumRay;

uniform mat4 MVP;
uniform mat4 inverseP;

void main() {
	gl_Position = MVP * vec4(inPosition, 1.0f);
	uv = (inPosition.xy+1)*0.5;
	vec4 ray = inverseP*vec4(inPosition.xy, -1.0f, 1.0f); //This works because Quad is in range [-1, 1]

    // unproject the frustum corner from NDC to view space
	frustumRay = (ray / ray.w).xyz;

	// z-normalize this vector
    frustumRay = frustumRay / frustumRay.z;

    //frustumRay = vec3(frustumRay.z);
}
