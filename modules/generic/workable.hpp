#pragma once

namespace Project {
    /**
     * @brief class that does work...
     * 
     */
    class Workable {
    public:
        /**
         * @brief do work (on your self), includes calls that should reside only on the main thread
         * 
         */
        virtual void MainThreadWork() {}

        /**
         * @brief all other work actions...
         * 
         */
        virtual void ThreadWork() {}

        /**
         * @brief do expensive non-main-thread work
         * 
         */
        virtual void ExpensiveThreadWork() {}
    };
}