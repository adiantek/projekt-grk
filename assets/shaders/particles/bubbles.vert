#version 300 es

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;

out vec2 TexCoords;
flat out int instanceID;

uniform vec4 particlePositionsAndLife[200];
uniform mat4 cameraMatrix;
uniform vec3 cameraDirection;
uniform vec3 cameraUp;

void main()
{
    gl_Position = cameraMatrix * vec4(particlePositionsAndLife[gl_InstanceID].xyz + (cameraDirection * vertexPosition.x * 0.02) + (cameraUp * vertexPosition.y * 0.02), 1.0);
    instanceID = gl_InstanceID;
    TexCoords = vertexTexCoord;
}