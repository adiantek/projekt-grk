#version 300 es

layout(location = 0) in vec4 vertexPosition;

uniform mat4 transformation;

out vec4 fragPos;

void main()
{
	gl_Position = transformation * vertexPosition;
	fragPos = transformation * vertexPosition;
}
