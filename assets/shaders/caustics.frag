#version 300 es

precision highp float;

const vec2 resolution = vec2(1024.0);
const float bias = 0.003;

uniform sampler2D colorTexture;
uniform sampler2D normalSampler;
uniform sampler2D caustics;

in vec3 position;
in vec2 texturePosition;
in vec3 viewDirectionTS;
in vec3 lightDirectionTS;
in vec3 positionLS;
in float lightIntensity;

out vec4 fragColor;

float blur(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
    vec2 off1 = vec2(1.3846153846) * direction;
    vec2 off2 = vec2(3.2307692308) * direction;
    float intensity = texture(image, uv).x * 0.2270270270;
    intensity += texture(image, uv + (off1 / resolution)).x * 0.3162162162;
    intensity += texture(image, uv - (off1 / resolution)).x * 0.3162162162;
    intensity += texture(image, uv + (off2 / resolution)).x * 0.0702702703;
    intensity += texture(image, uv - (off2 / resolution)).x * 0.0702702703;
    return intensity;
}

float computeCaustics(float lightIntensity, sampler2D caustics, vec3 positionLS) {
    float computedLightIntensity = 0.0;
    float causticsDepth = texture(caustics, positionLS.xy).w;

    if (causticsDepth > positionLS.z - bias) {
        float causticsIntensity = 0.5 * (
            blur(caustics, positionLS.xy, resolution, vec2(0.0, 0.5)) +
            blur(caustics, positionLS.xy, resolution, vec2(0.5, 0.0))
        );
        computedLightIntensity = 0.5 + 0.2 * lightIntensity + causticsIntensity * smoothstep(0.0, 1.0, lightIntensity);
    } else {
        float computedLightIntensity = 0.0;
        vec2 texelSize = 1.0 / vec2(textureSize(caustics, 0));
        computedLightIntensity += causticsDepth - bias > texture(caustics, positionLS.xy + vec2(-1, -1) * texelSize).r ? 1.0 : 0.0;
        computedLightIntensity += causticsDepth - bias > texture(caustics, positionLS.xy + vec2(-1, 0) * texelSize).r ? 1.0 : 0.0;
        computedLightIntensity += causticsDepth - bias > texture(caustics, positionLS.xy + vec2(-1, 1) * texelSize).r ? 1.0 : 0.0;
        computedLightIntensity += causticsDepth - bias > texture(caustics, positionLS.xy + vec2(0, -1) * texelSize).r ? 1.0 : 0.0;
        computedLightIntensity += causticsDepth - bias > texture(caustics, positionLS.xy + vec2(0, 0) * texelSize).r ? 1.0 : 0.0;
        computedLightIntensity += causticsDepth - bias > texture(caustics, positionLS.xy + vec2(0, 1) * texelSize).r ? 1.0 : 0.0;
        computedLightIntensity += causticsDepth - bias > texture(caustics, positionLS.xy + vec2(1, -1) * texelSize).r ? 1.0 : 0.0;
        computedLightIntensity += causticsDepth - bias > texture(caustics, positionLS.xy + vec2(1, 0) * texelSize).r ? 1.0 : 0.0;
        computedLightIntensity += causticsDepth - bias > texture(caustics, positionLS.xy + vec2(1, 1) * texelSize).r ? 1.0 : 0.0;
        computedLightIntensity /= 9.0;
    }
    return computedLightIntensity;
}

void main() {
    vec3 objectColor = texture(colorTexture, vec2(texturePosition.x, 1.0 - texturePosition.y)).xyz;
    vec3 lightDirection = normalize(lightDirectionTS);
    vec3 viewDirection = normalize(viewDirectionTS);
    vec3 normal = normalize(vec3(texture(normalSampler, vec2(texturePosition.x, 1.0 - texturePosition.y))) * 2.0 - 1.0);
    vec3 reflected = reflect(-lightDirection, normal);

    float ambient = 0.2;
    float diffuse = max(0.0, dot(normal, lightDirection));
    float specular = pow(max(0.0, dot(reflected, viewDirection)), 10.0);

    float lightIntensity = computeCaustics(lightIntensity, caustics, positionLS);

    objectColor = mix(objectColor, ambient * objectColor + diffuse * lightIntensity * objectColor + specular * lightIntensity * vec3(1.0), 0.9);

    fragColor = vec4(objectColor, 1.0);
}