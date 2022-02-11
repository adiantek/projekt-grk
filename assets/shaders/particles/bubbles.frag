#version 300 es

precision highp float;

in vec2 TexCoords;
flat in int instanceID;
out vec4 color;

uniform sampler2D bubbleTexture;
uniform float distanceToSurface;
uniform vec4 particlePositionsAndLife[200];

vec4 colorBegin = vec4(0.556, 0.737, 0.960, 1.0);
vec4 colorEnd = vec4(0.439, 0.741, 0.941, 1.0);

void main()
{
    float life = particlePositionsAndLife[instanceID].w/5.0;
    life = clamp(life, 0.0, 1.0);
    // color = (texture(bubbleTexture, TexCoords) * colorBegin);

    vec4 tex = texture(bubbleTexture, TexCoords);

    if (distanceToSurface > 0.1 && tex.a > 0.5) {
        color = tex;
        // color = mix(colorBegin, colorEnd, life);
    } else {
        discard;
    }
}  