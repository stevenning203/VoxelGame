#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <thread>
#include <generic/workable.hpp>

namespace Project {
    /**
     * @brief a class to keep track of precise time
     * 
     */
    class Timer : public Workable {
    private:
        double last_time;
        double delta_time;
        std::chrono::high_resolution_clock::time_point point;
        int delta_ns_high_res;

        /**
         * @brief update the timer
         * 
         */
        void Update();
    public:
        Timer();
        
        virtual void MainThreadWork() override;

        virtual void ThreadWork() override;

        double GetDeltaTime();

        /**
         * @brief sleep this thread
         * 
         * @param ms milliseconds
         */
        void Sleep(int ms);

        /**
         * @brief first high resolution begin
         * 
         */
        void HighResolutionMeasureFirst();

        /**
         * @brief second high resolution finish
         * 
         */
        void HighResolutionMeasureSecond();

        /**
         * @brief get delta from begin to end of high res timer
         * 
         * @return int NANOSECONDS, -1 if never measured.
         */
        int GetDeltaHighResolution();
    };
}