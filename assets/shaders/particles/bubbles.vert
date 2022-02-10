#version 300 es

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;

out vec2 TexCoords;
flat out int instanceID;

uniform vec4 particlePositionsAndLife[200];
uniform mat4 cameraMatrix;
uniform mat4 cameraDirection;

void main()
{
    // mat4 modelMatrix = mat4(
    //     vec4( 1.0, 0.0, 0.0, 0.0),
    //     vec4( 0.0, 1.0, 0.0, 0.0),
    //     vec4( 0.0, 0.0, 1.0, 0.0),
    //     vec4( particlePositionsAndLife[gl_InstanceID], 1.0) );

    // mat4 modelView = cameraMatrix * modelMatrix;

    // modelView[0][0] = 1.0;
    // modelView[0][1] = 0.0;
    // modelView[0][2] = 0.0;
    // modelView[1][0] = 0.0;
    // modelView[1][1] = 1.0;
    // modelView[1][2] = 0.0;
    // modelView[2][0] = 0.0;
    // modelView[2][1] = 0.0;
    // modelView[2][2] = 1.0;

    // gl_Position = (modelView * vec4(vertexPosition, 1.0));

    // gl_Position = cameraMatrix * cameraDirection * vec4(particlePositionsAndLife[gl_InstanceID].xyz + (vertexPosition * 0.02), 1.0);
    gl_Position = cameraMatrix * vec4(particlePositionsAndLife[gl_InstanceID].xyz + (vertexPosition * 0.02), 1.0);
    instanceID = gl_InstanceID;
    TexCoords = vertexTexCoord;
}