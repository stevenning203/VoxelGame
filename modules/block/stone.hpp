#pragma once

#include "block.hpp"

namespace Project {
    /**
     * @brief stone block
     * 
     */
    class Stone : public Block {
    public:
        Stone() = default;
        
        virtual int GetID() const override;

        virtual float Hardness() const override;
    };
}