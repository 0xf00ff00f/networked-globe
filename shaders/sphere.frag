#version 450 core

uniform vec4 color;

out vec4 frag_color;

in float shade;

void main(void)
{
    frag_color = shade * color;
}
