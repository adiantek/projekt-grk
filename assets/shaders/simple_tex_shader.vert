#version 300 es

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTex;

uniform mat4 transformation;

out vec2 uv;

void main()
{
	uv = vertexTex;
	gl_Position = transformation * vec4(vertexPosition, 1.0);
}