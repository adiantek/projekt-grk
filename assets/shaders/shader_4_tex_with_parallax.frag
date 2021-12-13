#version 300 es
precision highp float;

//uniform vec3 lightDir;
uniform vec3 lightPos;
uniform vec3 cameraPos;

uniform float heightScale;

uniform sampler2D colorTexture;
uniform sampler2D normalSampler;
uniform sampler2D depthSampler;

in vec3 interpNormal;
in vec3 fragPos;
in vec2 vertexTexCoord2;

in vec3 viewDirTS;
in vec3 lightDirTS;
in vec3 viewPos;
out vec4 FragColor;

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

void main()
{
	vec3 V = normalize(viewDirTS);
	vec2 vertexTexCoords = parallaxMapping(vertexTexCoord2, V, depthSampler, heightScale);

    if(vertexTexCoords.x > 1.0 || vertexTexCoords.y > 1.0 || vertexTexCoords.x < 0.0 || vertexTexCoords.y < 0.0)
        discard;
	vec3 objectColor = vec3(texture(colorTexture, vertexTexCoords));

	vec3 lightDir = normalize(lightDirTS);
	vec3 normal = normalize(vec3(texture(normalSampler, vertexTexCoords)) * 2.0 - 1.0);
	vec3 R = reflect(-normalize(lightDir),normal);
	
	float specular = 0.3 * pow(max(0.0,dot(R,V)),10.0);
	float diffuse = max(0.0,dot(normal,normalize(lightDir)));
	FragColor = vec4(mix(objectColor,objectColor*diffuse+vec3(1.0)*specular,0.9), 1.0);
	//gl_FragColor = vec4(vec3(texture(normalSampler, vertexTexCoord2)) * 2 - 1, 1.0);
}
