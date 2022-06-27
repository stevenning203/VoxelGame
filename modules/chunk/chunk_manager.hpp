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
         * @brief Generate the world for some chunks near spawn
         * 
         */
        void WorldGen();

        std::vector<Chunk>& GetChunks();

        /**
         * @brief Suggest a remesh on all the visible chunks
         * 
         */
        void SuggestRemesh();
    };
}