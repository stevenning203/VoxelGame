#pragma once
#include "block.hpp"

namespace Project {
    /**
     * @brief air block in the game
     * 
     */
    class AirBlock : public Block {
    public:
        AirBlock();
        
        virtual bool IsOpaque() override;

        virtual bool SkipRender() override;

        virtual int GetID() override;

        virtual bool IsSolid() override;

        virtual bool IsSelectable() override;
    };
}