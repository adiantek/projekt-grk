#version 300 es

in vec3 pos;
in vec2 tex;

out vec2 fragPos;

void main()
{
	gl_Position = vec4(pos, 1.0);
	fragPos = tex;
}
