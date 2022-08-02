#pragma once

#include <glm/glm.hpp>
#include <physics/entity.hpp>
#include <generic/workable.hpp>

namespace Project {
    class Camera;
    class Item;

    /**
     * @brief Player class
     * 
     */
    class Player : public Entity {
    private:
        Item* in_hand;

    public:
        Player() = default;

        void SetPosition(const float x, const float y, const float z);

        void SetPosition(const glm::vec3& vec);

        void SetDirection(const float x, const float y, const float z);

        void SetDirection(const glm::vec3& vec);

        /**
         * @brief return the item that the player has in their hand, may be nullptr(none)
         * 
         * @return Item* 
         */
        Item* InHand();
    };
}