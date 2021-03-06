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
        Player* player;

        /**
         * @brief enforce player and voxel collisions
         * 
         * @param cm 
         * @param p 
         */
        void EnforcePlayerVoxelCollision();

        /**
         * @brief move the camera to the player (right now it will be vice versa for now...)
         * 
         */
        void UpdatePlayerCamera();

    public:
        /**
         * @brief Construct a new World Collision Handler objectdefault
         * 
         */
        WorldCollisionHandler(ChunkManager* chunk_manager, Camera* camera, MouseHandler* mouse, Player* player);

        virtual void MainThreadWork() override;

        virtual void ThreadWork() override;
    };
}