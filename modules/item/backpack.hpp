#pragma once

#include <vector>

namespace Project {
    class Item;
    class Pile;

    /**
     * @brief backpack that holds piles of items
     * 
     */
    class Backpack {
        std::vector<Pile> piles;
        
    public:
        /**
         * @brief push an item into the backpack
         * 
         * @param item 
         */
        void Push(const Item& item); 
    };
}