#version 330 core

in vec4 vertex_color;
flat in uint tc_index;

out vec4 FragColor;

uniform sampler2D texture_sampler;

vec2 indices[4] = vec2[](
    vec2(0.f, 1.f),
    vec2(1.f, 1.f),
    vec2(0.f, 0.f),
    vec2(1.f, 0.f)
);

void main() {
    //FragColor = vec4(0.7f, 0.4f, 0.35f, 1.f) * vertex_color;
    FragColor = texture(texture_sampler, vertex_color.xy);
}