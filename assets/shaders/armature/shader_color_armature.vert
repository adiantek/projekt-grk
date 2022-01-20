#version 300 es

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;
layout(location = 5) in vec3 vertexJoints;
layout(location = 6) in vec3 vertexWeights;

out vec3 interpNormal;
out vec3 color;
out vec2 interpTexCoord;

// Max joints allowed in a skeleton (20)
uniform mat4 jointTransforms[20];
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;

void main() {
    vec4 localPos = vec4(0.0);
    vec4 normal = vec4(0.0);
    vec3 _color = vec3(0.0);

    // Max joints allowed per vertex (3)
    for (int i = 0; i < 3; i++) {
        mat4 jointTransform = jointTransforms[int(vertexJoints[i])];
        vec4 posePosition = jointTransform * vec4(vertexPosition, 1.0);
        localPos += posePosition * vertexWeights[i];

        // vec4 worldNormal = jointTransform * vec4(vertexNormal, 0.0);
        // normal += worldNormal * vertexWeights[i];
        _color.x += vertexJoints[i] / 10.0;
    }

    gl_Position = modelViewProjectionMatrix * localPos;
    // interpNormal = (modelMatrix * vec4(vertexNormal, 0.0)).xyz;
    interpTexCoord = vertexTexCoord;
    color = _color;

    // Simple color
    // gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);
	interpNormal = (modelMatrix * vec4(vertexNormal, 0.0)).xyz;

} 