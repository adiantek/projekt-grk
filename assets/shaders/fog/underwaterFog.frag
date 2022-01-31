#version 300 es

precision highp float;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;

in vec2 interpTexCoord;

out vec4 FragColor;

// ----- COLORS -----

// 1) COLOR: light turquoise
// vec3 fogColor = vec3(0.525, 0.905, 0.933);

// 2) COLOR: sea color
vec3 fogColor = vec3(0.262, 0.662, 0.839);

void main()
{
  vec4 colorBuffer = texture(screenTexture, interpTexCoord);
  float depth = texture(depthTexture, interpTexCoord).r;

  // ----- FOG FATNESS -----

  // lighter fog
  // float visibility = ((exp((depth * density) -95.0) + (depth * 10.0)) * 0.01) + 0.05;

  // fatter fog
  // float visibility = ((exp((depth * 100.0) -95.0) + (depth * 20.0)) * 0.01) + 0.05;

  // current fog
  float visibility = ((exp((depth * 100.0) -95.0) + exp(depth * 3.5)) * 0.01) + 0.05;
  
  visibility = clamp(visibility, 0.0, 1.0);

  FragColor = mix(colorBuffer, vec4(fogColor, 1.0), visibility);
}