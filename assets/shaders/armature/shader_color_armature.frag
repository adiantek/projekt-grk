#version 300 es

precision highp float;
uniform vec3 objectColor;
uniform vec3 lightDir;

in vec3 color;
in vec3 interpNormal;
out vec4 FragColor;

void main() {

	vec3 normal = normalize(interpNormal);
	// float diffuse = max(dot(normal, -lightDir), 0.3);
	FragColor = vec4(color, 1.0);

}
