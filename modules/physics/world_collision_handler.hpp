#pragma once

namespace Project {
    class WorldCollisionHandler {
    public:
        void EnforcePlayerVoxelCollision(ChunkManager& cm, Player& p);
    };
}