#version 300 es
precision highp float;

uniform vec3 objectColor;
//uniform vec3 lightDir;
uniform vec3 lightPos;
uniform vec3 cameraPos;

in vec3 interpNormal;
in vec3 fragPos;
out vec4 FragColor;

void main()
{
	vec3 normal = normalize(interpNormal);
	vec3 V = normalize(cameraPos-fragPos);
	float coef = max(0.0,dot(V,normal));
	FragColor = vec4(mix(objectColor,vec3(0.2,0.5,0.0),1.0-coef), 1.0);
}
