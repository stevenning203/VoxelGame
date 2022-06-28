#pragma once

#include <item/item.hpp>

namespace Project {
    /**
     * @brief a block in the game
     * 
     */
    class Block : public Item {
    public:
        virtual bool IsOpaque() = 0;

        /**
         * @brief Return whether or not this block should be skipped when rendering
         * (aka it is not a type of visible block)
         * 
         * @return true yes, skip
         * @return false no, please render
         */
        virtual bool SkipRender() = 0;
    };
}