#version 450 core

layout(location=0) in vec3 position;
layout(location=1) in float vert_tex_coord;

uniform mat4 mvp;

out float tex_coord;

void main(void)
{
    gl_Position = mvp*vec4(position, 1.0);
    tex_coord = vert_tex_coord;
}
