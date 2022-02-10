#version 300 es

precision highp float;

const float bias = 0.0005;
const float PI = 3.14159265359;

uniform sampler2D colorTexture[3];
uniform sampler2D normalSampler[3];
uniform sampler2D caustics;
uniform sampler2D depthMap[3];
uniform sampler2D aoMap[3];
uniform sampler2D roughnessMap[3];
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform float waterHeight;
uniform bool modeSwitch;

in vec3 position;
in vec2 texturePosition;
in vec3 viewDirectionTS;
in vec3 lightDirectionTS;
in vec3 positionLS;
in float lightIntensity;
in vec3 worldPos;

out vec4 fragColor;

vec4 getColor(sampler2D tex[3], vec2 uv) {
    const float water = 192.0;
    const float sand = 128.0;
    const float middle = (water + sand) / 2.0;

    // `smooth` is keyword?
    const float smooth_water = 2.0;
    const float smooth_sand = 8.0;

    if (worldPos.y > middle) {
        vec4 a = texture(tex[1], uv);
        vec4 b = texture(tex[0], uv);
        float sm = worldPos.y - water + smooth_water;
        sm = sm / (smooth_water * 2.0);
        sm = clamp(sm, 0.0, 1.0);
        return mix(a, b, sm);
    } else {
        vec4 a = texture(tex[2], uv);
        vec4 b = texture(tex[1], uv);
        float sm = worldPos.y - sand + smooth_sand;
        sm = sm / (smooth_sand * 2.0);
        sm = clamp(sm, 0.0, 1.0);
        return mix(a, b, sm);
    }
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

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir, sampler2D depthMap[3], float heightScale) {
    // Number of iterations based on angle to view direction
    float numLayers = mix(8.0, 32.0, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    // Size of every iteration step on depth
    float layerDepth = 1.0 / numLayers;
    // Size of every iteration change in texture coords
    vec2 deltaTexCoords = (viewDir.xy / viewDir.z * heightScale) / numLayers;
    // Initial itaration values
    float currentLayerDepth = 1.0;
    vec2  currentTexCoords = texCoords;
    float currentDepthMapValue = getColor(depthMap, currentTexCoords).r;
    // Iterate until intersect with texture
    while(currentLayerDepth > currentDepthMapValue) {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = getColor(depthMap, currentTexCoords).r;  
        currentLayerDepth -= layerDepth;
    }
    // Previous coords
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
    // Weight for linear interpolation between before and afer intersection
    float weight = (currentLayerDepth - currentDepthMapValue) / (layerDepth - currentDepthMapValue - getColor(depthMap, prevTexCoords).r);
    // Return interpolated point of intersection with texture
    return prevTexCoords * weight + currentTexCoords * (1.0 - weight);
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
        for(int x = -2; x <= 2; ++x) {
            for(int y = -2; y <= 2; ++y) {
                if (texture(caustics, positionLS.xy + vec2(x, y) * texelSize).w > positionLS.z - bias) {
                    computedLightIntensity += 1.0;
                }  
            }    
        }
        computedLightIntensity /= 25.0;
    }
    return computedLightIntensity;
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

void main() {
    vec3 lightDirection = normalize(lightDirectionTS);
    vec3 viewDirection = normalize(viewDirectionTS);
    vec2 textureCoords = parallaxMapping(vec2(texturePosition.x , texturePosition.y), viewDirection, depthMap, 0.05);
    
    vec3 normal = normalize(vec3(getColor(normalSampler, textureCoords)) * 2.0 - 1.0);
    vec3 objectColor = getColor(colorTexture, textureCoords).xyz;

    float computedLightIntensity = 1.0;

    if(position.y < waterHeight) {
        computedLightIntensity = computeCaustics(computedLightIntensity, caustics, positionLS);
    }
    if (!modeSwitch) {
        vec3 color = PBR(
            normal, viewDirection, objectColor, vec3(0.0),
            0.0, getColor(roughnessMap, textureCoords).r,
            getColor(aoMap, textureCoords).r, lightDirection,
            vec3(1.0) * computedLightIntensity, 1.0, vec3(0.05)
        );
        fragColor = vec4(color, 1.0);
    } else {
        vec3 reflected = reflect(-lightDirection, normal);

        float ambient = 0.1;
        float diffuse = 0.9 * computedLightIntensity * max(0.0, dot(normal, lightDirection));
        float specular = 0.4 * computedLightIntensity * pow(max(0.0, dot(reflected, viewDirection)), 10.0);

        objectColor = mix(objectColor, (ambient + diffuse) * objectColor + specular * vec3(1.0), 0.9);

        fragColor = vec4(objectColor, 1.0);
    }
}