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
        
        virtual bool IsOpaque() override;

        virtual bool SkipRender() override;

        virtual int GetID() override;
    };
}