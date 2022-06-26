#version 330 core

layout(location = 0) in vec3 layout_position;

uniform mat4 projection_matrix;
uniform mat4 mvp_matrix;
uniform mat4 view_matrix;
uniform mat4 test;
uniform mat4 model_matrix;
uniform mat4 chunk_matrix;

out vec4 vertex_color;

void main() {
    gl_Position = projection_matrix * view_matrix * chunk_matrix * vec4(layout_position, 1.f);
    vertex_color = vec4(layout_position, 1.f);
}