#version 450 core

uniform vec4 color;

out vec4 frag_color;

void main(void)
{
    frag_color = color;
}