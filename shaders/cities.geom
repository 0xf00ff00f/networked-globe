#version 450 core

layout(points) in;
layout(triangle_strip, max_vertices=4) out;

uniform mat4 mvp;

in world_vertex {
    vec3 position;
} gs_in[];

out sprite_vertex {
    vec2 tex_coord;
} gs_out;

void main(void)
{
    const vec3 n = normalize(gs_in[0].position);
    const vec3 s = normalize(cross(vec3(0.0, 1.0, 0.0), n));
    const vec3 u = cross(s, n);

    const vec3 p = 1.01 * n;
    const float size = 0.05;

    gl_Position = mvp*vec4(p - size*s - size*u, 1.0);
    gs_out.tex_coord = vec2(0.0, 0.0);
    EmitVertex();

    gl_Position = mvp*vec4(p - size*s + size*u, 1.0);
    gs_out.tex_coord = vec2(0.0, 1.0);
    EmitVertex();

    gl_Position = mvp*vec4(p + size*s - size*u, 1.0);
    gs_out.tex_coord = vec2(1.0, 0.0);
    EmitVertex();

    gl_Position = mvp*vec4(p + size*s + size*u, 1.0);
    gs_out.tex_coord = vec2(1.0, 1.0);
    EmitVertex();
}
