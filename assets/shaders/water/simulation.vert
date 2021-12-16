#version 300 es

layout(location = 0) in vec3 vertexPosition;

out vec2 texturePosition;

void main() {
    texturePosition = vertexPosition.xy * 0.5 + 0.5;

    gl_Position = vec4(vertexPosition, 1.0);
}