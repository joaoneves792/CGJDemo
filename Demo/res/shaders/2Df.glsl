#version 330 core

in vec2 uv;

uniform sampler2D renderedTexture;

out vec4 color;

void main() {

    /*Render depth buffer*/
    /*float z = texture(renderedTexture, uv).r;
    float n = 1.0;                                // the near plane
    float f = 30.0;                               // the far plane
    float c = (2.0 * n) / (f + n - z * (f - n));  // convert to linear values
    color.rgb = vec3(c);
    color.a = 1.0f;
    */

    color.rgba = texture(renderedTexture, uv).rgba;
}
