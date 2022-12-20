#pragma once

namespace Project {
    /**
     * @brief Physics behaviour.
     * 
     * Update is called immediately each frame.
     * 
     * FixedUpdate is called based on a fixed interval, such that it will be called n times per second, where n is defined by the parent
     * PhysicsEngine
     * 
     */
    class PhysicsBehaviour {
    public:
        /**
         * @brief update this behaviour. is called immediately, so that it is possible for it to be called either 0..inf times per second
         * 
         */
        virtual void Update() {}

        /**
         * @brief FIXED update this behaviour. is called such that it averages out to approximately n times per second, where n is defined by the parent physicsengine
         * 
         */
        virtual void FixedUpdate() {}
    };
}