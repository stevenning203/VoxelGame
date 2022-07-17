#pragma once

#include <glm/glm.hpp>

namespace Project {
    class ChunkManager;

    /**
     * @brief DDA-Style raycaster
     * 
     */
    class DDACaster {
    public:
        /**
         * @brief Construct a new DDACaster object default constructor
         * 
         */
        DDACaster() = default;

        /**
         * @brief generate a raycast from origin with direction. gives the row, col of the collided blocks if true is returned.
         * 
         * @param origin origin of the raycast
         * @param direction direction of the raycast
         * @param distance dist
         * @param world the world/chunk manager
         * @param write_to_row_collision row of the collision ray - abs
         * @param write_to_col_collision col of the collision ray - abs
         * @param write_to_y_collision y of the collision ray - abs
         * @param write_to_row_backtrack row of the backtrack raytrace - abs
         * @param write_to_col_backtrack column of the backtrack raytrace - abs
         * @param write_to_y_backtrack y of the collision ray - abs
         * @return true [yes, there was a collision]
         * @return false [no, there was not a collision]
         */
        bool Cast(const glm::vec3& origin, const glm::vec3& direction, float distance,
        ChunkManager& world,
        int& write_to_row_collision,
        int& write_to_col_collision,
        int& write_to_y_collision,
        int& write_to_row_backtrack,
        int& write_to_col_backtrack,
        int& wirte_to_y_backtrack);
    };
}