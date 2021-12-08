#version 300 es

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 1) in vec3 vertexNormal;

uniform mat4 transformation;
uniform mat4 modelMatrix;
out vec3 interpNormal;
out vec3 fragPos;

void main()
{
	gl_Position = transformation * vec4(vertexPosition, 1.0);
	interpNormal = (modelMatrix*vec4(vertexNormal,0)).xyz;
	fragPos = (modelMatrix*vec4(vertexPosition,1)).xyz;
}
