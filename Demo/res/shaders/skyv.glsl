#version 330 core
in vec3 aPos;

out vec3 TexCoords;

uniform mat4 MVP;

void main()
{
    TexCoords = vec3(aPos.x, -aPos.y, aPos.z);
    gl_Position = (MVP * vec4(aPos, 1.0f)).xyww;
}