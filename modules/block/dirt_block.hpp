#pragma once
#include "block.hpp"

namespace Project {
    /**
     * @brief grass block in the game
     * 
     */
    class DirtBlock : public Block {
    public:
        DirtBlock();
        
        virtual int GetID() const override;
    };
}