#version 300 es
precision highp float;

uniform vec3 color;
out vec4 FragColor;

void main()
{
	FragColor = vec4(color, 1.0);
}