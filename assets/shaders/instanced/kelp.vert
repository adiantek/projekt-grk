#version 300 es

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

out vec2 uv;
out vec3 position;
out vec3 viewDirectionTS;
out vec3 lightDirectionTS;
out vec3 positionLS;
out float lightIntensity;

uniform mat4 transformation;
uniform mat4 lightTransformation;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform vec3 lightDirection;
uniform int textureSize;
uniform sampler2D matrices;
uniform float time;

void main()
{
	vec3 animatedVertexPosition = vertexPosition;

	uv = vertexTexCoord;
	int xpos = gl_InstanceID * 4 % textureSize;
	int ypos = gl_InstanceID * 4 / textureSize;
	mat4 modelMatrix = mat4(
		texelFetch(matrices, ivec2(xpos + 0, ypos), 0),
		texelFetch(matrices, ivec2(xpos + 1, ypos), 0),
		texelFetch(matrices, ivec2(xpos + 2, ypos), 0),
		texelFetch(matrices, ivec2(xpos + 3, ypos), 0)
	);
    
	animatedVertexPosition.x += sin(vertexPosition.z * 2.0 + (time * 1.75)) * 0.05 * vertexPosition.z;

	gl_Position = transformation * modelMatrix * vec4(animatedVertexPosition, 1.0);

	position = (modelMatrix * vec4(animatedVertexPosition, 1.0)).xyz;

	vec3 tangent = (modelMatrix * vec4(vertexTangent, 0.0)).xyz;
	vec3 bitangent = (modelMatrix * vec4(vertexBitangent, 0.0)).xyz;
    vec3 normal = (modelMatrix * vec4(vertexNormal, 0.0)).xyz;
	mat3 tbn = transpose(mat3(tangent, bitangent, normal));

    viewDirectionTS = tbn * normalize(cameraPosition - position);
    lightDirectionTS = tbn * normalize(lightPosition - position);

    vec4 lightRelativePosition = lightTransformation * modelMatrix * vec4(animatedVertexPosition, 1.0);

    positionLS = 0.5 + lightRelativePosition.xyz / lightRelativePosition.w * 0.5;
    lightIntensity = -dot(normalize(lightDirection), normalize(vertexNormal));
}