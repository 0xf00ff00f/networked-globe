#version 450 core

uniform vec4 color;
uniform float tex_offset;

out vec4 frag_color;

in float tex_coord;

void main(void)
{
    float t = 1.0 - tex_coord + tex_offset;
    float alpha = smoothstep(0.95, 1.0, t);
    frag_color = vec4(color.rgb, alpha);
}
