#version 450 core

uniform vec4 color;

in sprite_vertex {
    vec2 tex_coord;
} fs_in;

out vec4 frag_color;

void main(void)
{
    const float l = 2.0 * length(fs_in.tex_coord - vec2(0.5));
    const float r = 0.5;
    const float thickness = 0.2;
    const float d = 1.0 - smoothstep(0.0, thickness, abs(l - r));
    frag_color = vec4(color.rgb, d);
}
