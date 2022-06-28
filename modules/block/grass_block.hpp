#pragma once
#include "block.hpp"

namespace Project {
    /**
     * @brief grass block in the game
     * 
     */
    class GrassBlock : public Block {
        virtual bool IsOpaque() override;

        virtual bool SkipRender() override;
    };
}