#version 330 core

in vec4 vertex_color;
in vec2 texture_coordinate;

out vec4 FragColor;

uniform sampler2D texture_sampler;

void main() {
    //FragColor = vec4(0.7f, 0.4f, 0.35f, 1.f) * vertex_color;
    FragColor = texture(texture_sampler, texture_coordinate);
}