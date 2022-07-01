#pragma once

namespace Project {
    class ChunkManager;
    class Player;
    
    class WorldCollisionHandler {
    public:
        void EnforcePlayerVoxelCollision(ChunkManager& cm, Player& p);
    };
}