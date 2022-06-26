#version 330 core

layout(location = 0) in vec3 layout_position;
layout(location = 1) in int block_data;

uniform mat4 projection_matrix;
uniform mat4 mvp_matrix;
uniform mat4 view_matrix;
uniform mat4 test;
uniform mat4 model_matrix;
uniform mat4 chunk_matrix;

out vec4 vertex_color;

vec3 vertices[] = {
    vec3(-0.5f, 0.5f, 0.5f), 
    vec3(0.5f, 0.5f, 0.5f), 
    vec3(-0.5f, -0.5f, 0.5f), 
    vec3(0.5f, -0.5f, 0.5f), 
    vec3(-0.5f, 0.5f, -0.5f), 
    vec3(0.5f, 0.5f,- 0.5f), 
    vec3(-0.5f, -0.5f, -0.5f), 
    vec3(0.5f, -0.5f, -0.5f), 
};

void main() {
    int index = block_data >> 16;
    int pos = block_data & 0b0000_0000_0000_0000_1111_1111_1111_1111;
    int x = (pos & 0b1111_0000_0000_0000) >> 12;
    int y = pos & 0b0000_0000_1111_1111;
    int z = (pos & 0b0000_1111_0000_0000) >> 8;
    //gl_Position = projection_matrix * view_matrix * chunk_matrix * vec4(layout_position, 1.f);
    gl_Position = projection_matrix * view_matrix * chunk_matrix * vec4(x, y, z, 1.f);
    vertex_color = vec4(layout_position, 1.f);
}