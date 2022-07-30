#pragma once

#include <glm/glm.hpp>

namespace Project {
    /**
     * @brief A class with a position
     * 
     */
    class Positionable {
    protected:
        glm::vec3 position{0};
        glm::vec3 direction{1.f};
    public:
        /**
         * @brief Get the Position object
         * 
         * @return glm::vec3& the reference to the positiion vector.
         */
        virtual const glm::vec3& GetPosition();

        virtual const glm::vec3& GetDirection();
    };
}