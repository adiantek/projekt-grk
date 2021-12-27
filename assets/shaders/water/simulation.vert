#version 300 es

uniform vec2 transition;
uniform float scale;

layout(location = 0) in vec3 vertexPosition;

out vec2 position;

void main() {
    position = vertexPosition.xy * scale + transition;
    gl_Position = vec4(vertexPosition, 1.0);
}