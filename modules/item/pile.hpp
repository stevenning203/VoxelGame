#pragma once

namespace Project {
    class Item;

    /**
     * @brief a pile of items
     * 
     */
    class Pile {
        Item* item;
        int count;
    public:
        int Count();
    };
}