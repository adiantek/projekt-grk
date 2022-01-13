#version 300 es

precision highp float;

const float causticsFactor = 0.25;

in vec3 oldPosition;
in vec3 newPosition;
in float waterDepth;
in float depth;

out vec4 fragColor;

void main() {
    float causticsIntensity = 0.0;

    if (depth >= waterDepth) {
        float oldArea = length(dFdx(oldPosition)) * length(dFdy(oldPosition));
        float newArea = length(dFdx(newPosition)) * length(dFdy(newPosition));

        float ratio;

        if (newArea == 0.) {
            ratio = 2.0e+20;
        } else {
            ratio = oldArea / newArea;
        }

        causticsIntensity = causticsFactor * ratio;
    }
    fragColor = vec4(causticsIntensity, 0.0, 0.0, depth);
}