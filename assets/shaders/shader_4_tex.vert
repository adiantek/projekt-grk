#version 300 es

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

uniform mat4 transformation;
uniform mat4 modelMatrix;

uniform vec3 lightPos;
uniform vec3 cameraPos;

out vec3 interpNormal;
out vec3 fragPos;
out vec2 vertexTexCoord2;

out vec3 viewDirTS;
out vec3 lightDirTS;

void main()
{
	gl_Position = transformation * vec4(vertexPosition, 1.0);
	interpNormal = (modelMatrix*vec4(vertexNormal,0)).xyz;
	fragPos = (modelMatrix*vec4(vertexPosition,1)).xyz;
	vertexTexCoord2 = vertexTexCoord;
	
	vec3 normal = (modelMatrix * vec4(vertexNormal, 0.0)).xyz;
	vec3 tangent = (modelMatrix * vec4(vertexTangent, 0.0)).xyz;
	vec3 bitangent = (modelMatrix * vec4(vertexBitangent, 0.0)).xyz;
	mat3 tbn = transpose(mat3(tangent, bitangent, normal));

	vec3 viewDir = normalize(cameraPos-fragPos);
	vec3 lightDir = normalize(lightPos-fragPos);
	
	viewDirTS = tbn * viewDir;
	lightDirTS = tbn * lightDir;
}
