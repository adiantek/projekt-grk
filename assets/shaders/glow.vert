#version 300 es

in vec4 pos;
in vec2 tex;

out vec2 fragPos;

void main()
{
	gl_Position = pos;
	fragPos = tex;
}
