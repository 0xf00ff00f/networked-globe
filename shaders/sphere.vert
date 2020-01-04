#version 450 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in float vert_shade;

uniform mat4 mvp;

out float shade;

void main(void)
{
    shade = vert_shade;
    gl_Position = mvp*vec4(position, 1.0);
}
