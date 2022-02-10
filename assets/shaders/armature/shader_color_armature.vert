#version 300 es

const int MAX_JOINTS_MER_MODEL = 50;
const int MAX_JOINTS_PER_VERTEX = 3;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;
layout(location = 5) in uvec3 vertexJoints;
layout(location = 6) in vec3 vertexWeights;

out vec3 viewDirectionTS;
out vec3 lightDirectionTS;
out vec3 interpNormal;
out vec2 texCoords;
out vec3 position;
out vec3 positionLS;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

uniform mat4 jointTransforms[MAX_JOINTS_MER_MODEL];
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 lightTransformation;

void main() {
    vec4 localPos = vec4(0.0);

    for (int i = 0; i < MAX_JOINTS_PER_VERTEX; i++) {
        mat4 jointTransform = jointTransforms[vertexJoints[i]];
        vec4 posePosition = jointTransform * vec4(vertexPosition, 1.0);
        localPos += posePosition * vertexWeights[i];
    }

    vec3 tangent = (modelMatrix * vec4(vertexTangent, 0.0)).xyz;
	vec3 bitangent = (modelMatrix * vec4(vertexBitangent, 0.0)).xyz;
    vec3 normal = (modelMatrix * vec4(vertexNormal, 0.0)).xyz;
    mat3 tbn = transpose(mat3(tangent, bitangent, normal));

    position = (modelMatrix * vec4(vec3(localPos), 1.0)).xyz;
    
    viewDirectionTS = tbn * normalize(cameraPosition - position);
    lightDirectionTS = tbn * normalize(lightPosition - position);

    vec4 lightRelativePosition = lightTransformation * modelMatrix * localPos;

    positionLS = 0.5 + lightRelativePosition.xyz / lightRelativePosition.w * 0.5;

    gl_Position = modelViewProjectionMatrix * localPos;
    interpNormal = (modelMatrix * vec4(vertexNormal, 0.0)).xyz;
    texCoords = vertexTexCoord;
}