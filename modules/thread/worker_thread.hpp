#pragma once

#include <vector>

namespace Project {
    class Workable;
    class Timer;

    class WorkerThread {
    public:
        WorkerThread();

        /**
         * @brief add a job that will be loop called on this thread
         * 
         * @param workable 
         */
        void AddWorkable(Workable* workable);

        /**
         * @brief start the work
         * 
         */
        void Start();

        /**
         * @brief get the number of seconds since last frame
         * 
         * @return double 
         */
        double GetDeltaTimeSeconds();
    private:
        std::vector<Workable*> jobs;
        Timer* timer;
    };
}