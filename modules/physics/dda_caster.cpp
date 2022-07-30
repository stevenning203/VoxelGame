#include "dda_caster.hpp"
#include <glm/glm.hpp>
#include <chunk/chunk_manager.hpp>
#include <cmath>
#include <block/block.hpp>
#include <iostream>
#include <generic/debug.hpp>

bool Project::DDACaster::Cast(const glm::vec3& origin, const glm::vec3& direction, float distance, ChunkManager& world, int& write_to_row_collision, int& write_to_col_collision, int& write_to_y_collision, int& write_to_row_backtrack, int& write_to_col_backtrack, int& write_to_y_backtrack) {
    glm::vec3 ray_start = origin;
    auto sqr = [](float x) -> float { return x*x; };
    glm::vec3 ray_unit_step_size{std::sqrtf(1.f + sqr(direction.y / direction.x) + sqr(direction.z / direction.x)),
    std::sqrtf(1.f + sqr(direction.x / direction.y) + sqr(direction.z / direction.y)),
    std::sqrtf(1.f + sqr(direction.x / direction.z) + sqr(direction.y / direction.z))};
    glm::ivec3 map_check = glm::floor(ray_start);
    glm::ivec3 begin_compare = map_check;
    glm::vec3 ray_length{0.f};
    glm::ivec3 step{0};
    if (direction.x < 0) {
        step.x = -1;
        ray_length.x = (ray_start.x - static_cast<float>(map_check.x)) * ray_unit_step_size.x;
    } else {
        step.x = 1;
        ray_length.x = (static_cast<float>(map_check.x + 1) - ray_start.x) * ray_unit_step_size.x;
    }
    if (direction.y < 0) {
        step.y = -1;
        ray_length.y = (ray_start.y - static_cast<float>(map_check.y)) * ray_unit_step_size.y;
    } else {
        step.y = 1;
        ray_length.y = (static_cast<float>(map_check.y + 1) - ray_start.y) * ray_unit_step_size.y;
    }
    if (direction.z < 0) {
        step.z = -1;
        ray_length.z = (ray_start.z - static_cast<float>(map_check.z)) * ray_unit_step_size.z;
    } else {
        step.z = 1;
        ray_length.z = (static_cast<float>(map_check.z + 1) - ray_start.z) * ray_unit_step_size.z;
    }
    float current_distance = 0.0f;
    int last_op = -1;
    while (current_distance < distance) {
        if (world.BlockExists(map_check.x, map_check.y, map_check.z) && world.AskBlockProperty(map_check.x, map_check.y, map_check.z, &Block::IsSelectable)) {
            write_to_row_collision = map_check.x;
            write_to_col_collision = map_check.z;
            write_to_y_collision = map_check.y;
            if (last_op >= 0 && map_check != begin_compare) {
                glm::ivec3 delta{0};
                if (last_op == 0) {
                    delta.x -= step.x;
                } else if (last_op == 1) {
                    delta.y -= step.y;
                } else if (last_op == 2) {
                    delta.z -= step.z;
                }
                glm::ivec3 x = map_check + delta;
                write_to_col_backtrack = x.z;
                write_to_row_backtrack = x.x;
                write_to_y_backtrack = x.y;
            }
            return true;
        }
        if (ray_length.x < ray_length.y && ray_length.x < ray_length.z) {
            map_check.x += step.x;
            current_distance = ray_length.x;
            ray_length.x += ray_unit_step_size.x; 
            last_op = 0;
        } else if (ray_length.y < ray_length.x && ray_length.y < ray_length.z) {
            map_check.y += step.y;
            current_distance = ray_length.y;
            ray_length.y += ray_unit_step_size.y; 
            last_op = 1;
        } else {
            map_check.z += step.z;
            current_distance = ray_length.z;
            ray_length.z += ray_unit_step_size.z; 
            last_op = 2;
        }
    }
    return false;
}