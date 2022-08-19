#pragma once

#include <generic/workable.hpp>
#include <glm/vec3.hpp>

namespace Project {
    class ChunkManager;
    class Player;
    class DDACaster;
    class Camera;
    class MouseHandler;
    class Entity;
    
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
        DDACaster* caster;

        /**
         * @brief move the camera to the player (right now it will be vice versa for now...)
         * 
         */
        void UpdatePlayerCamera();

        /**
         * @brief update the player position so that the player will have moved according to their velocity. If
         * the player hits a wall, they will stop and slide.
         * 
         */
        void UpdatePlayerPosition();

        /**
         * @brief perform a swept AABB collision detection
         * 
         * @param entity the entity that is moving
         * @param x the world x of the block
         * @param y world y
         * @param z world z
         * @return float the time it will take to collide (normalized between 0.f and 1.f, 1.f meaning that no collison occurs)
         */
        static float SweptAABB(const Entity& entity, const int x, const int y, const int z);

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