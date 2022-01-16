#version 300 es

const float waterAirETA = 0.7504126;
const int maxIterations = 100;

uniform sampler2D heightMap;
uniform sampler2D normalMap;
uniform sampler2D environmentMap;
uniform mat4 transformation;
uniform mat4 modelMatrix;
uniform vec3 lightDirection;
uniform float height;

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec2 texturePosition;

out vec3 oldPosition;
out vec3 newPosition;
out float waterDepth;
out float depth;

void main() {
    vec4 heightInfo = texture(heightMap, vec2(texturePosition.x, 1.0 - texturePosition.y));

    vec3 waterPosition = vec3(vertexPosition.xy + heightInfo.xy, vertexPosition.z + heightInfo.z + height);
    vec3 waterNormal = normalize(texture(normalMap, vec2(texturePosition.x, 1.0 - texturePosition.y)).xyz);

    oldPosition = waterPosition;

    vec4 projectedWaterPosition = transformation * modelMatrix * vec4(waterPosition, 1.0);

    vec3 refracted = refract(normalize(lightDirection), waterNormal, waterAirETA);
    vec3 refractedDirection = (transformation * vec4(refracted, 0.0)).xyz;

    waterDepth = projectedWaterPosition.z / projectedWaterPosition.w * 0.5 + 0.5;

    float factor = 1.0 / (float(textureSize(environmentMap, 0).x) * length(refractedDirection.xy));

    float currentDepth = projectedWaterPosition.z;
    vec2 currentPosition = projectedWaterPosition.xy;

    vec4 environment = texture(environmentMap, 0.5 + 0.5 * currentPosition);

    vec2 deltaDirection = refractedDirection.xy * factor;
    float deltaDepth = refractedDirection.z * factor;

    for(int i = 0; i <= maxIterations; ++i) {
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