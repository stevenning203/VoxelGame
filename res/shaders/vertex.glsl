#version 330 core

layout(location = 0) in vec3 test_vertex;
layout(location = 1) in uint block_data;

uniform mat4 projection_matrix;
uniform mat4 mvp_matrix;
uniform mat4 view_matrix;
uniform float chunk_offset_x;
uniform float chunk_offset_z;

out vec2 texture_coordinate;

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

const vec2 tcs[4] = vec2[](
    vec2(0.f, 1.f),
    vec2(1.f, 1.f),
    vec2(0.f, 0.f),
    vec2(1.f, 0.f)
);

const float n_atlas_count = 12.f;
const float n_atlas_vert = 1.f;

void main() {
    uint index = (block_data & uint(983040)) >> 16;
    uint pos = block_data & uint(65535);
    uint x = (pos & uint(61440)) >> 12;
    uint y = pos & uint(255);
    uint z = (pos & uint(3840)) >> 8;
    uint tc_index = (block_data & uint(4026531840)) >> 28;
    uint block_id = (block_data & uint(267386880)) >> 20;
    texture_coordinate = vec2(tcs[tc_index].x / n_atlas_count, tcs[tc_index].y) + vec2(float(block_id) / n_atlas_count, 0.f);
    float x_pos = float(x);
    float y_pos = float(y);
    float z_pos = float(z);
    x_pos += chunk_offset_x;
    z_pos += chunk_offset_z;
    //gl_Position = projection_matrix * view_matrix * (vec4(x_pos, y_pos, z_pos, 1.f) + vec4(vertices[index], 1.f));
    vec3 vertex_base = vertices[index];
    //gl_Position = projection_matrix * view_matrix * vec4(test_vertex, 1.f);
    //gl_Position = vec4(vertex_base, 1.f);
    gl_Position = projection_matrix * view_matrix * vec4(vertex_base.x + x_pos, vertex_base.y + y_pos, vertex_base.z + z_pos , 1.f);
}