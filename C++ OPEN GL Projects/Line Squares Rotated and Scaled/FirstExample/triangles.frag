#version 430 core

in vec3 myColor;
out vec4 frag_color;

void main()
{
	frag_color = vec4(myColor,1.0);
}