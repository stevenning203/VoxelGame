#version 330 core

in vec4 vertex_color;

out vec4 FragColor;

void main() {
    FragColor = vec4(0.2f, 0.5f, 0.25f, 1.f) * vertex_color;
}