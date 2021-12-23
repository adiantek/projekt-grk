#version 300 es

layout(location = 0) in vec4 vertexPosition;

out vec4 fragPos;

void main()
{
	gl_Position = vertexPosition;
	fragPos = vertexPosition;
}
