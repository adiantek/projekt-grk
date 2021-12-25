#version 300 es

precision highp float;

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texturePosition;

uniform sampler2D heightMap;
uniform sampler2D normalMap;
uniform mat4 transformation;
uniform mat4 model;
uniform vec3 cameraPosition;
uniform vec3 lightPosition;

out vec3 normal;
out vec3 viewDirection;
out vec3 fragmentPosition;

void main() {
    vec3 transformedPosition = position + texture(heightMap, texturePosition).xyz;
    fragmentPosition = (model * vec4(transformedPosition, 1.0)).xyz;

    normal = normalize((model * vec4(texture(normalMap, texturePosition).xyz, 0.0)).xyz);
    viewDirection = normalize(cameraPosition - fragmentPosition);

    gl_Position = transformation * vec4(transformedPosition, 1.0);
}