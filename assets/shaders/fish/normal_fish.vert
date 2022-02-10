#version 300 es

uniform mat4 transformation;
uniform mat4 lightTransformation;
uniform mat4 modelMatrix;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform vec3 lightDirection;
uniform float time;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

out vec3 position;
out vec2 texturePosition;
out vec3 viewDirectionTS;
out vec3 lightDirectionTS;
out vec3 positionLS;
out float lightIntensity;

void main() {
    vec3 animatedVertexPosition = vertexPosition;
    animatedVertexPosition.x += sin(vertexPosition.y * 0.5 + (time * 5.0)) * 0.5;

    gl_Position = transformation * vec4(animatedVertexPosition, 1.0);

    position = (modelMatrix * vec4(animatedVertexPosition, 1.0)).xyz;
    texturePosition = vertexTexCoord;

	vec3 tangent = (modelMatrix * vec4(vertexTangent, 0.0)).xyz;
	vec3 bitangent = (modelMatrix * vec4(vertexBitangent, 0.0)).xyz;
    vec3 normal = (modelMatrix * vec4(vertexNormal, 0.0)).xyz;
	mat3 tbn = transpose(mat3(tangent, bitangent, normal));

    viewDirectionTS = tbn * normalize(cameraPosition - position);
    lightDirectionTS = tbn * normalize(lightPosition - position);

    vec4 lightRelativePosition = lightTransformation * modelMatrix * vec4(animatedVertexPosition, 1.0);

    positionLS = 0.5 + lightRelativePosition.xyz / lightRelativePosition.w * 0.5;
    lightIntensity = -dot(normalize(lightDirection), normalize(vertexNormal));
}