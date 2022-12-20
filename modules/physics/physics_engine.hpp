#pragma once

#include <generic/workable.hpp>

namespace Project {
    class PhysicsBehaviour;

    class PhysicsEngine : public Workable {
    public:
        /**
         * @brief Construct a new Physics Engine object
         * 
         * @param fixed_updates_per_second the amount of fixed updates that should be done in each second
         */
        PhysicsEngine(const int fixed_updates_per_second);

        virtual void ThreadWork() override;

        virtual void MainThreadWork() override;

        virtual void ExpensiveThreadWork() override;
    };
}