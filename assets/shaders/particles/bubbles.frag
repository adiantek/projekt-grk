#version 300 es

precision highp float;

in vec2 TexCoords;
flat in int instanceID;
out vec4 color;

// uniform sampler2D sprite;
uniform float distanceToSurface;
uniform vec4 particlePositionsAndLife[100];

vec4 colorBegin = vec4(0.666, 0.074, 0.101, 1.0);
vec4 colorEnd = vec4(0.094, 0.168, 0.705, 1.0);

void main()
{
    float life = particlePositionsAndLife[instanceID].w/5.0;
    life = clamp(life, 0.0, 1.0);
    // color = (texture(sprite, TexCoords) * colorBegin);
    color = mix(colorBegin, colorEnd, life);
}  