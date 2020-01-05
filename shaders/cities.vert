#version 450 core

layout(location=0) in vec3 position;
layout(location=1) in float vert_alpha;

out world_vertex {
    vec3 position;
    float alpha;
} vs_out;

void main(void)
{
    vs_out.position = position;
    vs_out.alpha = vert_alpha;
}
