#version 300 es
precision highp float;

//uniform vec3 lightDir;
uniform vec3 lightPos;
uniform vec3 cameraPos;

uniform sampler2D colorTexture;
uniform sampler2D normalSampler;
uniform sampler2D caustics;

in vec3 interpNormal;
in vec3 fragPos;
in vec2 vertexTexCoord2;

in vec3 viewDirTS;
in vec3 lightDirTS;
in vec3 lightPosition;
in float lightIntensity;
out vec4 FragColor;

const float bias = 0.003;
const vec2 resolution = vec2(1572.0);

float blur(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
  float intensity = 0.;
  vec2 off1 = vec2(1.3846153846) * direction;
  vec2 off2 = vec2(3.2307692308) * direction;
  intensity += texture2D(image, uv).x * 0.2270270270;
  intensity += texture2D(image, uv + (off1 / resolution)).x * 0.3162162162;
  intensity += texture2D(image, uv - (off1 / resolution)).x * 0.3162162162;
  intensity += texture2D(image, uv + (off2 / resolution)).x * 0.0702702703;
  intensity += texture2D(image, uv - (off2 / resolution)).x * 0.0702702703;
  return intensity;
}

void main()
{
	vec3 objectColor = vec3(texture(colorTexture, vec2(vertexTexCoord2.x, 1.0 - vertexTexCoord2.y)));

	vec3 lightDir = normalize(lightDirTS);
	vec3 V = normalize(viewDirTS);
	vec3 normal = normalize(vec3(texture(normalSampler, vec2(vertexTexCoord2.x, 1.0 - vertexTexCoord2.y))) * 2.0 - 1.0);
	vec3 R = reflect(-normalize(lightDir),normal);
	
	float specular = pow(max(0.0,dot(R,V)),10.0);
	float diffuse = max(0.0,dot(normal,normalize(lightDir)));

    vec3 objectColorFinal = mix(objectColor,objectColor*diffuse+vec3(1.0)*specular,0.9);

    float computedLightIntensity = 0.5;

    computedLightIntensity += 0.2 * lightIntensity;

    float causticsDepth = texture2D(caustics, lightPosition.xy).w;

    if (causticsDepth > lightPosition.z - bias) {
        // Percentage Close Filtering
        float causticsIntensity = 0.5 * (
            blur(caustics, lightPosition.xy, resolution, vec2(0.0, 0.5)) +
            blur(caustics, lightPosition.xy, resolution, vec2(0.5, 0.0))
        );

        //objectColorFinal += causticsIntensity;
        computedLightIntensity += causticsIntensity * smoothstep(0.0, 1.0, lightIntensity);
    } else {
        computedLightIntensity = 0.3;
    }
	FragColor = vec4(objectColorFinal * computedLightIntensity, 1.0);
}