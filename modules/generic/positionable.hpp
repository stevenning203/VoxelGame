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
        glm::vec3 velocity;
    public:
        /**
         * @brief Get the Position object
         * 
         * @return glm::vec3& the reference to the positiion vector.
         */
        virtual const glm::vec3& GetPosition() const;

        /**
         * @brief Get the Direction object
         * 
         * @return const glm::vec3& 
         */
        virtual const glm::vec3& GetDirection() const;

        /**
         * @brief Get the Velocity object
         * 
         * @return const glm::vec3& 
         */
        virtual const glm::vec3& GetVelocity() const;

        /**
         * @brief add velocity to the position
         * 
         */
        virtual void AddVelocity();
    };
}