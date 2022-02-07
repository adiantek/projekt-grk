#version 300 es

uniform mat4 transformation;
uniform mat4 modelMatrix;

layout(location = 0) in vec3 vertexPosition;

out vec4 worldPosition;

void main() {
    worldPosition = modelMatrix * vec4(vertexPosition, 1.0);

    vec4 projectedPosition = transformation * vec4(vertexPosition, 1.0);
    worldPosition.w = projectedPosition.z * 0.5 + 0.5;

    gl_Position = projectedPosition;
}