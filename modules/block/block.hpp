#pragma once

#include <item/item.hpp>

namespace Project {
    /**
     * @brief a block in the game
     * 
     */
    class Block : public Item {
    public:
        bool IsOpaque();
    };
}