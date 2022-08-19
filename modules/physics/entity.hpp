#pragma once
#include <generic/positionable.hpp>

namespace Project {
    /**
     * @brief entity in the game
     * 
     */
    class Entity : public Positionable {
    public:
        virtual const glm::vec3& GetDimensions() const = 0;
    };
}