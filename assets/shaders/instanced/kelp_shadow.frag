#version 300 es

precision highp float;

uniform sampler2D texAlbedo;
in vec2 uv;
in vec4 worldPosition;

out vec4 FragColor;

void main()
{
	if (texture(texAlbedo, uv).a < 0.75) {
		discard;
	}
	FragColor = worldPosition;
}