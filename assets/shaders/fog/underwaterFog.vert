#version 300 es

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 vertexTexCoord;

out vec2 interpTexCoord;

void main()
{
  gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);

  interpTexCoord = vertexTexCoord;
}