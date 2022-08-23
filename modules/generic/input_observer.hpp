#pragma once

namespace Project {
    class Input;

    /**
     * @brief an observer for inputs
     * 
     */
    class InputObserver {
    public:
        /**
         * @brief notify this observer that something has changed in the input sphere
         * 
         * @param input 
         */
        virtual void Notify(const Input& input) = 0;
    };
}