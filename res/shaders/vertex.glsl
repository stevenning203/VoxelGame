#version 330 core

layout(location = 0) in vec3 layout_position;
layout(location = 1) in uint block_data;

uniform mat4 projection_matrix;
uniform mat4 mvp_matrix;
uniform mat4 view_matrix;
uniform float chunk_offset_x;
uniform float chunk_offset_z;

out vec4 vertex_color;

const vec3 vertices[8] = vec3[](
    vec3(0.f, 1.f, 1.f), 
    vec3(1.f, 1.f, 1.f), 
    vec3(0.f, 0.f, 1.f), 
    vec3(1.f, 0.f, 1.f), 
    vec3(0.f, 1.f, 0.f), 
    vec3(1.f, 1.f, 0.f), 
    vec3(0.f, 0.f, 0.f), 
    vec3(1.f, 0.f, 0.f)
);

void main() {
    uint index = block_data >> 16;
    uint pos = block_data & uint(65535);
    uint x = (pos & uint(61440)) >> 12;
    uint y = pos & uint(255);
    uint z = (pos & uint(3840)) >> 8;
    //gl_Position = projection_matrix * view_matrix * chunk_matrix * vec4(layout_position, 1.f);
    gl_Position = projection_matrix * view_matrix * (vec4(x + chunk_offset_x, y, z + chunk_offset_z, 1.f) + vec4(vertices[index], 1.f));
    //gl_Position = vec4(layout_position, 1.f);
    vertex_color = vec4(vertices[index], 1.f);
    //vertex_color = vec4(1.f, 1.f, 1.f, 1.f);
}