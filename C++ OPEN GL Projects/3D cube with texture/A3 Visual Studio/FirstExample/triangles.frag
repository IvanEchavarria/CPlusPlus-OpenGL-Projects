#version 430 core

in vec3 myColor;
in vec2 textCoord;
out vec4 frag_colour;

uniform sampler2D texture0;

void main() {
    frag_colour = texture(texture0,textCoord);
}

