#version 300 es

layout(location = 0) in vec3 vertexPosition;

uniform mat4 transformation;

void main()
{
	gl_Position = transformation * vec4(vertexPosition, 1.0);
}