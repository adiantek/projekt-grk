#version 300 es

precision highp float;

const float PI = 3.14159265359;
const float bias = 0.005;

uniform sampler2D albedoTexture;
uniform sampler2D normalTexture;
uniform sampler2D aoTexture;
uniform sampler2D roughnessTexture;
uniform sampler2D metallicTexture;
uniform sampler2D caustics;

uniform float waterHeight;

in vec3 viewDirectionTS;
in vec3 lightDirectionTS;
in vec3 interpNormal;
in vec2 texCoords;
in vec3 position;
in vec3 positionLS;

out vec4 fragColor;

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return a2 / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    return NdotV /  (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float ggx2 = GeometrySchlickGGX(max(dot(N, V), 0.0), roughness);
    float ggx1 = GeometrySchlickGGX(max(dot(N, L), 0.0), roughness);

    return ggx1 * ggx2;
}

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

vec3 PBR(vec3 normal, vec3 view, vec3 albedo, vec3 F0, float metallic, float roughness, float ao, vec3 lightDirection, vec3 lightColor, float lightDistance, vec3 ambientStrength) {
    F0 = mix(F0, albedo, metallic);
    // reflectance equation
    vec3 Lo = vec3(0.0);
    // calculate light radiance
    vec3 H = normalize(view + lightDirection);
    vec3 radiance = lightColor * (1.0 / (lightDistance * lightDistance));
    // cook-torrance brdf
    vec3 F = fresnelSchlick(max(dot(H, view), 0.0), F0);

    vec3 kD = (vec3(1.0) - F) * (1.0 - metallic);

    float NdotL = max(dot(normal, lightDirection), 0.0);

    vec3 numerator = DistributionGGX(normal, H, roughness) * GeometrySmith(normal, view, lightDirection, roughness) * F;
    float denominator = 4.0 * max(dot(normal, view), 0.0) * NdotL + 0.0001;
    vec3 specular = numerator / denominator;
    // add to outgoing radiance Lo
    Lo += (kD * albedo / PI + specular) * radiance * NdotL;

    vec3 ambient = ambientStrength * albedo * ao;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    return color;
}

float computeCaustics(float lightIntensity, sampler2D caustics, vec3 positionLS) {
    // If someone is drawing from outside of space
    if(positionLS.x > 1.0 || positionLS.y > 1.0 || positionLS.x < 0.0 || positionLS.y < 0.0) {
        positionLS.z = 0.0;
        //return 1.0;
    }
    float computedLightIntensity = 0.0;
    float causticsDepth = texture(caustics, positionLS.xy).w;

    if (causticsDepth > positionLS.z - bias) {
        vec2 texelSize = vec2(textureSize(caustics, 0));
        float causticsIntensity = 0.5 * (
            blur(caustics, positionLS.xy, texelSize, vec2(0.0, 0.5)) +
            blur(caustics, positionLS.xy, texelSize, vec2(0.5, 0.0))
        );
        computedLightIntensity = 0.5 + 0.2 * lightIntensity + causticsIntensity * smoothstep(0.0, 1.0, lightIntensity);
    } else {
        vec2 texelSize = 1.0 / vec2(textureSize(caustics, 0));
        for(int x = -1; x <= 1; ++x) {
            for(int y = -1; y <= 1; ++y) {
                computedLightIntensity += causticsDepth - bias > texture(caustics, positionLS.xy + vec2(x, y) * texelSize).w ? 1.0 : 0.0;       
            }    
        }
        computedLightIntensity /= 9.0;
    }
    return computedLightIntensity;
}

void main() {

    vec2 invertedTexCoords = vec2(texCoords.x, 1.0 - texCoords.y);
    vec3 lightDirection = normalize(lightDirectionTS);
    vec3 viewDirection = normalize(viewDirectionTS);
	vec3 normal = texture(normalTexture, invertedTexCoords).rgb;
    vec3 albedo = texture(albedoTexture, invertedTexCoords).rgb;
    float metallic = texture(metallicTexture, invertedTexCoords).r;
    // float metallic = 1.0;
    float roughness = texture(roughnessTexture, invertedTexCoords).r;
    // float roughness = 0.0;
    float ao = texture(aoTexture, invertedTexCoords).r;

    // -- Caustics --
    float computedLightIntensity = 1.0;
    if (position.y < waterHeight) {
        computedLightIntensity = computeCaustics(computedLightIntensity, caustics, positionLS);
    }

    // -- PBR --
    vec3 color = PBR(
        normal, viewDirection, albedo, vec3(0.0),
        metallic, roughness, ao, lightDirection,
        vec3(1.0) * computedLightIntensity, 1.0, vec3(0.05)
    );

    // -- Final color
    fragColor = vec4(color, 1.0);

}