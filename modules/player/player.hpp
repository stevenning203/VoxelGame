#pragma once

#include <glm/glm.hpp>
#include <physics/entity.hpp>
#include <generic/workable.hpp>

namespace Project {
    class Camera;

    /**
     * @brief Player class
     * 
     */
    class Player : public Entity {

    public:
        Player() = default;

        void SetPosition(const float x, const float y, const float z);

        void SetPosition(const glm::vec3& vec);

        void SetDirection(const float x, const float y, const float z);

        void SetDirection(const glm::vec3& vec);
    };
}