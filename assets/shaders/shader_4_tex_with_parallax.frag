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

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir, sampler2D depthMap, float heightScale)
{ 
    // number of depth layers
    const float minLayers = 8.0;
    const float maxLayers = 32.0;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * heightScale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords.x -= deltaTexCoords.x;
        currentTexCoords.y += deltaTexCoords.y;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
    prevTexCoords.y -= 2.0 * deltaTexCoords.y;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main()
{
	vec3 V = normalize(viewDirTS);
	vec2 vertexTexCoords = ParallaxMapping(vertexTexCoord2, V, depthSampler, heightScale);

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
