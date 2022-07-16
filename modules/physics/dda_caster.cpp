#include "dda_caster.hpp"
#include <glm/glm.hpp>
#include <chunk/chunk_manager.hpp>
#include <cmath>
#include <block/block.hpp>
#include <iostream>

bool Project::DDACaster::Cast(const glm::vec3& origin, const glm::vec3& direction, const glm::vec3& end, ChunkManager& world, int& write_to_row_collision, int& write_to_col_collision, int& write_to_y_collision, int& write_to_row_backtrack, int& write_to_col_backtrack, int& write_to_y_backtrack) {
    glm::vec3 distance_vector = end - origin;
    float end_dist = glm::length(distance_vector);
    float dx = distance_vector.x;
    float dy = distance_vector.y;
    float dz = distance_vector.z;
    float hypotoneuse = std::sqrtf(dx * dx + dy * dy + dz * dz);
    float hypotoneuse_scaling_factors[] = {
        hypotoneuse / dx,
        hypotoneuse / dy,
        hypotoneuse / dz
    };

    auto norm = [](int a){
        return a > 0 ? 1 : -1;
    };

    int steps[] = {norm(distance_vector.x), norm(distance_vector.y), norm(distance_vector.z)};

    auto index_min = [](const glm::vec3& v){
        if (v.x < v.y) {
            if (v.x < v.z) {
                return 0;
            } else {
                return 2;
            }
        } else {
            if (v.y < v.z) {
                return 1;
            } else {
                return 2;
            }
        }
        return -1;
    };

    glm::ivec3 walker = origin;
    glm::vec3 walker_distances;

    if (direction.x < 0) {
        walker_distances.x = (origin.x - static_cast<float>(walker.x)) * steps[0];
    } else {
        walker_distances.x = (static_cast<float>(walker.x + 1) - origin.x) * steps[0];
    }
    if (direction.y < 0) {
        walker_distances.y = (origin.y - static_cast<float>(walker.y)) * steps[0];
    } else {
        walker_distances.y = (static_cast<float>(walker.y + 1) - origin.y) * steps[0];
    }
    if (direction.z < 0) {
        walker_distances.z = (origin.z - static_cast<float>(walker.z)) * steps[0];
    } else {
        walker_distances.z = (static_cast<float>(walker.z + 1) - origin.z) * steps[0];
    }

    while (true) {
        int index;
        index = index_min(walker_distances);
        walker_distances[index] += hypotoneuse_scaling_factors[index];
        walker[index] += steps[index];
        if (std::abs(walker_distances[index]) > end_dist) {
            break;
        }
        if (world.BlockExists(walker.x, walker.y, walker.z) && world.operator()(walker.x, walker.y, walker.z)->IsSelectable()) {
            // need to write to the ints here....
            write_to_row_collision = walker.x;
            write_to_y_collision = walker.y;
            write_to_col_collision = walker.z;
            return true;
        }
    }
    return false;
}