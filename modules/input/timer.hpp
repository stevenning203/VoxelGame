#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <thread>

namespace Project {
    /**
     * @brief a class to keep track of precise time
     * 
     */
    class Timer {
    private:
        double last_time;
        double delta_time;
    public:
        Timer();
        /**
         * @brief update the timer
         * 
         */
        void Update();

        double GetDeltaTime();

        /**
         * @brief sleep this thread
         * 
         * @param ms milliseconds
         */
        void Sleep(int ms);
    };
}