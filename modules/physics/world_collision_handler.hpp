#pragma once

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
    class WorldCollisionHandler {
    private:
        DDACaster* ray_caster;
    public:
        constexpr static float PLAYER_REACH = 5.f;

        /**
         * @brief Construct a new World Collision Handler objectdefault
         * 
         */
        WorldCollisionHandler();

        /**
         * @brief enforce player and voxel collisions
         * 
         * @param cm 
         * @param p 
         */
        void EnforcePlayerVoxelCollision(ChunkManager& cm, Player& p);

        /**
         * @brief allow the player to select blocks
         * 
         * @param cm 
         * @param c 
         * @param reach 
         */
        void EnablePlayerBlockDestruction(ChunkManager&cm, Camera& c, MouseHandler& mouse, float reach);
    };
}