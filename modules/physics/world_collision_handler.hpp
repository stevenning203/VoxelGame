#pragma once

#include <generic/workable.hpp>

namespace Project {
    class ChunkManager;
    class Player;
    class DDACaster;
    class Camera;
    class MouseHandler;
    
    /**
     * @brief handle world collisions/physics
     * 
     */
    class WorldCollisionHandler : public Workable {
    private:
        ChunkManager* chunk_manager;
        Camera* camera;
        MouseHandler* mouse;

        /**
         * @brief enforce player and voxel collisions
         * 
         * @param cm 
         * @param p 
         */
        void EnforcePlayerVoxelCollision();

    public:
        /**
         * @brief Construct a new World Collision Handler objectdefault
         * 
         */
        WorldCollisionHandler(ChunkManager* chunk_manager, Camera* camera, MouseHandler* mouse);

        virtual void MainThreadWork() override;

        virtual void ThreadWork() override;
    };
}