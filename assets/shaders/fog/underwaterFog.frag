#version 300 es

precision highp float;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;
uniform float distanceToSurface;

in vec2 interpTexCoord;

out vec4 FragColor;

vec3 fogColor = vec3(0.0, 0.0, 0.0);

// ----- COLORS -----

// 1) COLOR: sea color
vec3 fogColorLighter = vec3(0.203, 0.701, 0.898);
vec3 fogColorDarker = vec3(0.043, 0.321, 0.576);

// 1) COLOR: blue color
// vec3 fogColorLighter = vec3(0.356, 0.650, 0.984);
// vec3 fogColorDarker = vec3(0, 0.247, 0.678);

// 2) COLOR: sea color2
// vec3 fogColorLighter = vec3(0.262, 0.662, 0.839);
// vec3 fogColorDarker = vec3(0.050, 0.298, 0.627);

void main()
{
  vec4 colorBuffer = texture(screenTexture, interpTexCoord);
  float depth = texture(depthTexture, interpTexCoord).r;

//-----------------------------------------------------------------------------------------
  // depth = 1.0 - depth;
  // float lighterFog = exp(-pow(depth * 100.0, 2.0));
  // float fatterFog = exp(-pow(depth * 50.0, 1.2) - 0.05) + 0.05;
//-----------------------------------------------------------------------------------------
  float lighterFog = ((exp((depth * depth * 150.0) -143.0) + exp(depth * 5.0)) * 0.00075) + 0.12;
  float fatterFog = ((exp((depth * depth * 150.0) -143.0) + exp(depth * 5.0)) * 0.000715) + 0.36;
//-----------------------------------------------------------------------------------------
  
  lighterFog = clamp(lighterFog, 0.0, 1.0);
  fatterFog = clamp(fatterFog, 0.0, 1.0);
  
  float hightDistance = distanceToSurface/120.0;
  hightDistance = clamp(hightDistance, 0.0, 1.0);

  vec3 fogColor = mix(fogColorLighter, fogColorDarker, hightDistance);
  
  float visibility = mix(lighterFog, fatterFog, hightDistance);
  // visibility = clamp(visibility, 0.0, 1.0);
  
  if (distanceToSurface > 0.0) {
    FragColor = mix(colorBuffer, vec4(fogColor, 1.0), visibility);
  } else {
    FragColor = colorBuffer;
  }
}