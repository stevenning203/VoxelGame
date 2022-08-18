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
        
        virtual bool IsOpaque() const override;

        virtual bool SkipRender() const override;

        virtual int GetID() const override;

        virtual bool IsSolid() const override;

        virtual bool IsSelectable() const override;
    };
}