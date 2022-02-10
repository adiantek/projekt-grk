#version 300 es

precision highp float;

in vec4 worldPosition;

out vec4 fragColor;

void main() {
    fragColor = worldPosition;
}