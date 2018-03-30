#version 430 core
layout(location = 0) in vec2 vPosition;
layout(location = 1) in vec3 vColor;

out vec3 myColor;

uniform mat4 Model;

void main()
{
	myColor = vColor;
	gl_Position =  Model * vec4(vPosition, 0.0, 1.0);

}
