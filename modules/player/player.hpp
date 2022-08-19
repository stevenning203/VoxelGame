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
        constexpr static glm::vec3 PLAYER_DIMENSIONS{0.25f, 1.8f, 0.25f};
        Item* in_hand;
        bool flying_enabled = true;

    public:

        Player() = default;

        void SetPosition(const float x, const float y, const float z);

        void SetPosition(const glm::vec3& vec);

        void SetDirection(const float x, const float y, const float z);

        void SetDirection(const glm::vec3& vec);

        void SetVelocity(const float x, const float y, const float z);

        void SetVelocity(const glm::vec3& vec);

        virtual const glm::vec3& GetDimensions() const override;

        /**
         * @brief return the item that the player has in their hand, may be nullptr(none)
         * 
         * @return Item* 
         */
        Item* InHand();
    };
}