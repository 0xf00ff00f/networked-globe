#version 450 core

layout(location=0) in vec3 position;

out world_vertex {
    vec3 position;
} vs_out;

void main(void)
{
    vs_out.position = position;
}
