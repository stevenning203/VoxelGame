#pragma once
#include "block.hpp"

namespace Project {
    /**
     * @brief air block in the game
     * 
     */
    class AirBlock : public Block {
        virtual bool IsOpaque() override;

        virtual bool SkipRender() override;
    };
}