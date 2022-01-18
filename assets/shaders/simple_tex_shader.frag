#version 300 es
precision highp float;

uniform sampler2D textureSampler;
in vec2 uv;
out vec4 FragColor;

void main()
{
	FragColor = texture(textureSampler, uv);
}