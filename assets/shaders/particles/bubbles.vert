#version 300 es

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;

out vec2 TexCoords;
flat out int instanceID;

uniform vec4 particlePositionAndLife[100];
uniform mat4 cameraMatrix;
// uniform mat4 projection;
// uniform vec2 offset;

void main()
{
    // float scale = 10.0f;
    // TexCoords = vertex.zw;
    // gl_Position = projection * vec4((vertex.xy * scale) + offset, 0.0, 1.0);
    gl_Position = vec4(particlePositionAndLife[gl_InstanceID].xyz + vertexPosition, 1.0);
    instanceID = gl_InstanceID;
    TexCoords = vertexTexCoord;
}