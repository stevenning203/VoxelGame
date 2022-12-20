#pragma once

#include <generic/debug.hpp>

namespace Project {
    /**
     * @brief count seconds between frames
     * 
     */
    class Timer {
    public:
        Timer() = default;

        /**
         * @brief get the delta seconds since the last time that Update was called
         * 
         * @return double 
         */
        double GetDeltaSeconds();

        /**
         * @brief update the timer so that the delta seconds is up to date
         * 
         */
        void Update();
    private:
        double delta_time = 0.0;
        ntd::tp time_point;
    };
}