#pragma once

#include <generic/idable.hpp>

namespace Project {
    class Game;

    /**
     * @brief An item in the game that is in the inventory and (Can) be used if the child class overrides
     * the use virtual function.
     * 
     */
    class Item {
    public:
        enum class ToolTypeEnum {
            NONE
        };

        virtual void Use(Game& game);

        virtual int GetID() = 0;

        virtual ~Item() = default;

        virtual float BreakingPower();

        virtual ToolTypeEnum ToolType();
    };
}