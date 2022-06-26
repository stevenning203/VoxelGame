#pragma once

#include <glm/glm.hpp>

namespace Project {
    /**
     * @brief A class with a position
     * 
     */
    class Positionable {
    private:
        glm::vec3 position;
    public:
        /**
         * @brief Get the Position object
         * 
         * @return glm::vec3& the reference to the positiion vector.
         */
        glm::vec3& GetPosition();
    };
}