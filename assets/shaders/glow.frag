#version 300 es

precision highp float;

// source: https://github.com/UnlegitMC/FDPClient/blob/main/src/main/resources/assets/minecraft/fdpclient/shader/fragment/glow.frag

uniform sampler2D glowTexture;
uniform vec2 texelSize;

const vec3 color = vec3(1.0, 0.0, 1.0);
const float quality = 1.0;
const float radius = 4.0;
const float divider = 140.0;
const float maxSample = 10.0;

in vec2 fragPos;
out vec4 fragColor;

void main() {
    vec4 centerCol = texture(glowTexture, fragPos);

    if(centerCol.a != 0.0) {
        fragColor = vec4(centerCol.rgb, 0.0);
    } else {
        float alpha = 0.0;
        for (float x = -radius; x <= radius; x++) {
            for (float y = -radius; y <= radius; y++) {
                vec4 currentColor = texture(glowTexture, fragPos + vec2(texelSize.x * x * quality, texelSize.y * y * quality));
                if (currentColor.a != 0.0) {
                    alpha += divider > 0.0 ? max(0.0, (maxSample - distance(vec2(x, y), vec2(0.0))) / divider) : 1.0;
                }
            }
        }
        fragColor = vec4(color, alpha);
    }
}