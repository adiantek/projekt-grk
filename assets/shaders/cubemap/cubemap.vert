#version 300 es
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}