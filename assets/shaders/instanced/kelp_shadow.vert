#version 300 es

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec2 vertexTex;

out vec2 uv;
out vec4 worldPosition;

uniform mat4 transformation;
uniform int textureSize;
uniform sampler2D matrices;
uniform float time;

void main()
{
	vec3 animatedVertexPosition = vertexPosition;

	uv = vertexTex;
	int xpos = gl_InstanceID * 4 % textureSize;
	int ypos = gl_InstanceID * 4 / textureSize;
	mat4 modelMatrix = mat4(
		texelFetch(matrices, ivec2(xpos + 0, ypos), 0),
		texelFetch(matrices, ivec2(xpos + 1, ypos), 0),
		texelFetch(matrices, ivec2(xpos + 2, ypos), 0),
		texelFetch(matrices, ivec2(xpos + 3, ypos), 0)
	);
    
	animatedVertexPosition.x += sin(vertexPosition.z * 2.0 + (time * 1.75)) * 0.05 * vertexPosition.z;
    worldPosition = modelMatrix * vec4(animatedVertexPosition, 1.0);

    vec4 projectedPosition = transformation * modelMatrix * vec4(animatedVertexPosition, 1.0);
    worldPosition.w = projectedPosition.z * 0.5 + 0.5;

    gl_Position = projectedPosition;
}