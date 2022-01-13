#version 300 es

const float eta = 0.7504;
const int maxIterations = 100;

uniform sampler2D depthMap;
uniform sampler2D normalMap;
uniform sampler2D environmentMap;
uniform float deltaEnvTexture;
uniform mat4 transformation;
uniform mat4 rotation;
uniform vec3 light;

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec2 texturePosition;

out vec3 oldPosition;
out vec3 newPosition;
out float waterDepth;
out float depth;

void main() {
    vec4 depthInfo = texture(depthMap, texturePosition);
    vec4 normalInfo = texture(normalMap, texturePosition);

    vec3 waterPosition = vec3(vertexPosition.xy + depthInfo.xy, vertexPosition.z + depthInfo.b + 9.0);//vertexPosition + depthInfo.xyz;
    vec3 waterNormal = normalize(normalInfo.xyz);

    oldPosition = waterPosition;


    vec4 projectedWaterPosition = transformation * rotation * vec4(waterPosition, 1.0);

    vec2 currentPosition = projectedWaterPosition.xy;

    vec3 refracted = refract(normalize(light), waterNormal, eta);
    vec4 projectedRefractionVector = transformation * vec4(refracted, 1.0);

    vec3 refractedDirection = projectedRefractionVector.xyz;

    waterDepth = 0.5 + 0.5 * projectedWaterPosition.z / projectedWaterPosition.w;
    float currentDepth = projectedWaterPosition.z;
    vec4 environment = texture(environmentMap, 0.5 + 0.5 * currentPosition);

    float factor = deltaEnvTexture / length(refractedDirection.xy);

    vec2 deltaDirection = refractedDirection.xy * factor;
    float deltaDepth = refractedDirection.z * factor;

    for(int i=0; i<=maxIterations; ++i) {
        currentPosition += deltaDirection;
        currentDepth += deltaDepth;

        if (environment.w <= currentDepth) {
            break;
        }

        environment = texture(environmentMap, 0.5 + 0.5 * currentPosition);
    }

    newPosition = environment.xyz;

    vec4 projectedEnvPosition = transformation * vec4(newPosition, 1.0);
    depth = 0.5 + 0.5 * projectedEnvPosition.z / projectedEnvPosition.w;

    gl_Position = projectedEnvPosition;
}