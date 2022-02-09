#version 300 es

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec2 vertexTex;

out vec2 uv;

uniform mat4 transformation;
uniform int textureSize;
uniform sampler2D matrices;

void main()
{
	uv = vertexTex;
	int xpos = gl_InstanceID * 4 % textureSize;
	int ypos = gl_InstanceID * 4 / textureSize;
	mat4 locTransf = mat4(
		texelFetch(matrices, ivec2(xpos + 0, ypos), 0),
		texelFetch(matrices, ivec2(xpos + 1, ypos), 0),
		texelFetch(matrices, ivec2(xpos + 2, ypos), 0),
		texelFetch(matrices, ivec2(xpos + 3, ypos), 0)
	);
	gl_Position = transformation * locTransf * vec4(vertexPosition, 1.0);
}