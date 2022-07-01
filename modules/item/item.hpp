#pragma once

#include <generic/idable.hpp>

namespace Project {
    class Game;

    /**
     * @brief An item in the game that is in the inventory and (Can) be used if the child class overrides
     * the use virtual function.
     * 
     */
    class Item : public Identifiable<int> {
        virtual void Use(Game& game);
    };
}