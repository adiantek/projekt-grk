#version 300 es

uniform mat4 transformation;
uniform mat4 modelMatrix;
uniform float time;

layout(location = 0) in vec3 vertexPosition;

out vec4 worldPosition;

void main() {
    vec3 animatedVertexPosition = vertexPosition;
    animatedVertexPosition.x += sin(vertexPosition.y * 0.5 + (time * 5.0)) * 0.5;

    worldPosition = modelMatrix * vec4(animatedVertexPosition, 1.0);

    vec4 projectedPosition = transformation * vec4(animatedVertexPosition, 1.0);
    worldPosition.w = projectedPosition.z * 0.5 + 0.5;

    gl_Position = projectedPosition;
}