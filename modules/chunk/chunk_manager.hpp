#pragma once

#include <vector>

namespace Project {
    class Chunk;
    /**
     * @brief Manager for chunks
     * 
     */
    class ChunkManager {
        std::vector<Chunk> chunks;
    public:
        /**
         * @brief Generate the world
         * 
         */
        void WorldGen();

        std::vector<Chunk>& GetChunks();
    };
}