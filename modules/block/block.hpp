#pragma once

#include <item/item.hpp>

namespace Project {
    /**
     * @brief a block in the game
     * 
     */
    class Block : public Item {
    public:
        /**
         * @brief Indicate whether or not this block has transparency values
         * 
         * @return true this block is opaque so and blocks that it obscures should not have that face rendered.
         * @return false this block has transparency so blocks it obscures should still be rendered.
         */
        virtual bool IsOpaque();

        /**
         * @brief Return whether or not this block should be skipped when rendering
         * (aka it is not a type of visible block)
         * 
         * @return true yes, skip
         * @return false no, please render
         */
        virtual bool SkipRender();

        /**
         * @brief indicate whether or not this block is passable by entities
         * 
         * @return true solid, do not pass through
         * @return false unsolid, entities will pass through
         */
        virtual bool IsSolid();

        /**
         * @brief if a block is selectable (by the player)
         * 
         * @return true 
         * @return false 
         */
        virtual bool IsSelectable();
    };
}