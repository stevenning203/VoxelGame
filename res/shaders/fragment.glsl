#version 330 core

in vec4 vertex_color;

out vec4 FragColor;

void main() {
    FragColor = vec4(0.7f, 0.4f, 0.35f, 1.f) * vertex_color;
}