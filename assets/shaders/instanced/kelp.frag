#version 300 es

precision highp float;

uniform sampler2D texAlbedo;
in vec2 uv;

out vec4 FragColor;

void main()
{
	FragColor = texture(texAlbedo, uv);
}