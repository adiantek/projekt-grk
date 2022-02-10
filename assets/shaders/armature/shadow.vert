#version 300 es

const int MAX_JOINTS_MER_MODEL = 20;
const int MAX_JOINTS_PER_VERTEX = 3;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;
layout(location = 5) in uvec3 vertexJoints;
layout(location = 6) in vec3 vertexWeights;

uniform mat4 jointTransforms[MAX_JOINTS_MER_MODEL];
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;

out vec4 worldPosition;

void main() {
    vec4 localPos = vec4(0.0);

    for (int i = 0; i < MAX_JOINTS_PER_VERTEX; i++) {
        mat4 jointTransform = jointTransforms[vertexJoints[i]];
        vec4 posePosition = jointTransform * vec4(vertexPosition, 1.0);
        localPos += posePosition * vertexWeights[i];
    }
    worldPosition = modelMatrix * vec4(localPos.xyz, 1.0);

    vec4 projectedPosition = modelViewProjectionMatrix * localPos;
    worldPosition.w = projectedPosition.z * 0.5 + 0.5;

    gl_Position = projectedPosition;
}