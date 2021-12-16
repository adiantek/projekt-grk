#version 300 es

precision highp float;
precision highp int;

const float PI = 3.141592653589793;

uniform sampler2D depthMap;
uniform vec2 center;
uniform float radius;
uniform float strength;

in vec2 texturePosition;


void main() {
  /* Get vertex info */
  vec2 texPos = vec2(texturePosition.x, 1.0 - texturePosition.y);
  vec4 info = texture(depthMap, texPos);

  /* Add the drop to the height */
  float drop = max(0.0, 1.0 - length(center * 0.5 + 0.5 - texPos) / radius);
  drop = 0.5 - cos(drop * PI) * 0.5;
  info.r += drop * strength;

  gl_FragColor = info;
}