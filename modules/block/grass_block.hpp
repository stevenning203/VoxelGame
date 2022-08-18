#pragma once
#include "block.hpp"

namespace Project {
    /**
     * @brief grass block in the game
     * 
     */
    class GrassBlock : public Block {
    public:
        GrassBlock();
        
        virtual int GetID() const override;
    };
}