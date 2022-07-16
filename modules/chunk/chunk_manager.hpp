#pragma once

#include <vector>
#include <unordered_map>
#include <utility>
#include <memory>
#include "custom_pair_hasher.hpp"
#include <glm/glm.hpp>
#include <queue>

namespace Project {
    class Chunk;
    class CustomChunkPairHasher;
    class Block;
    class ChunkMeshManager;

    /**
     * @brief Manager for chunks
     * 
     */
    class ChunkManager {
        std::unordered_map<std::pair<int, int>, Chunk*, CustomChunkPairHasher> chunks;
        std::queue<std::pair<int, int>> chunk_generation_queue;
        ChunkMeshManager* partner;
        int radius;
    private:
        /**
         * @brief apply world generation to just one chunk
         * 
         */
        void GenerateChunk(const int row, const int col);

        /**
         * @brief accept the next chunk in the generation queue.
         * 
         */
        void NextInChunkQueue();
    public:

        /**
         * @brief Construct a new Chunk Manager object
         * 
         */
        ChunkManager(ChunkMeshManager* a);
        
        /**
         * @brief Generate the world for some chunks near spawn
         * 
         */
        void WorldGen();

        /**
         * @brief Remove out of vision chunks and add chunks that should be in viewable distance
         * 
         * @param position the position with which we want to update respect to
         */
        void UpdatePlayerVisibleChunks(glm::vec3& position);

        /**
         * @brief get the block at the specified WORLD coordinates.
         * 
         * @param x the x world coordinate
         * @param y the y world coordinate
         * @param z the z world coordinate
         * @return Block*& the reference to the block* at the specified location
         */
        Block*& operator()(const int x, const int y, const int z);

        /**
         * @brief return if the block exists or not with world coordinates
         * 
         * @param x 
         * @param y 
         * @param z 
         * @return true 
         * @return false 
         */
        bool BlockExists(const int x, const int y, const int z);

        /**
         * @brief get the chunk at the specified chunk coordinates
         * 
         * @param r row
         * @param c col
         * @return Chunk*& 
         */
        Chunk*& operator()(const int r, const int c);

        /**
         * @brief hint that the chunk at r c needs to be remeshed.
         * 
         * @param r 
         * @param c 
         */
        void HintRemeshing(const int r, const int c);

        /**
         * @brief return the STL iterator to the beginning
         * 
         * @return std::unordered_map<std::pair<int, int>, std::unique_ptr<Chunk>, CustomChunkPairHasher>::iterator 
         */
        std::unordered_map<std::pair<int, int>, Chunk*, CustomChunkPairHasher>::iterator begin();

        /**
         * @brief return the STL iterator to the end
         * 
         * @return std::unordered_map<std::pair<int, int>, std::unique_ptr<Chunk>, CustomChunkPairHasher>::iterator 
         */
        std::unordered_map<std::pair<int, int>, Chunk*, CustomChunkPairHasher>::iterator end();
    };
}