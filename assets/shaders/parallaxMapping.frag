#version 300 es
precision highp float;

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir, sampler2D depthMap, float heightScale) {
    // Number of iterations based on angle to view direction
    float numLayers = mix(8.0, 32.0, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    // Size of every iteration step on depth
    float layerDepth = 1.0 / numLayers;
    // Size of every iteration change in texture coords
    vec2 deltaTexCoords = (vec2(viewDir.x, -viewDir.y) / viewDir.z * heightScale) / numLayers;
    // Initial itaration values
    float currentLayerDepth = 1.0;
    vec2  currentTexCoords = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
    // Iterate until intersect with texture
    while(currentLayerDepth > currentDepthMapValue) {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
        currentLayerDepth -= layerDepth;  
    }
    // Previous coords
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
    // Weight for linear interpolation between before and afer intersection
    float weight = (currentLayerDepth - currentDepthMapValue) / (layerDepth - currentDepthMapValue - texture(depthMap, prevTexCoords).r);
    // Return interpolated point of intersection with texture
    return prevTexCoords * weight + currentTexCoords * (1.0 - weight);
}