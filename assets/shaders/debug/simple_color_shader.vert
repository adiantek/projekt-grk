#version 300 es

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;

uniform mat4 transformation;

out vec4 color;

void main()
{
	color = vertexColor;
	gl_Position = transformation * vec4(vertexPosition, 1.0);
}