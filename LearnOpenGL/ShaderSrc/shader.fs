#version 330 core
out vec4 color;

in vec3 our_color;
in vec2 our_tex_coord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    color = mix(texture(texture1, our_tex_coord), texture(texture2, vec2(1.0 - our_tex_coord.x, our_tex_coord.y)), 0.2);
}