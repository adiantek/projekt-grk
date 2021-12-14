#version 300 es
precision highp float;

//uniform vec3 lightDir;
uniform vec3 lightPos;
uniform vec3 cameraPos;

uniform sampler2D colorTexture;
uniform sampler2D normalSampler;

in vec3 interpNormal;
in vec3 fragPos;
in vec2 vertexTexCoord2;

in vec3 viewDirTS;
in vec3 lightDirTS;
out vec4 FragColor;

void main()
{
	vec3 objectColor = vec3(texture(colorTexture, vec2(vertexTexCoord2.x, 1.0 - vertexTexCoord2.y)));

	vec3 lightDir = normalize(lightDirTS);
	vec3 V = normalize(viewDirTS);
	vec3 normal = normalize(vec3(texture(normalSampler, vec2(vertexTexCoord2.x, 1.0 - vertexTexCoord2.y))) * 2.0 - 1.0);
	vec3 R = reflect(-normalize(lightDir),normal);
	
	float specular = pow(max(0.0,dot(R,V)),10.0);
	float diffuse = max(0.0,dot(normal,normalize(lightDir)));
	FragColor = vec4(mix(objectColor,objectColor*diffuse+vec3(1.0)*specular,0.9), 1.0);
	//gl_FragColor = vec4(vec3(texture(normalSampler, vertexTexCoord2)) * 2 - 1, 1.0);
}
