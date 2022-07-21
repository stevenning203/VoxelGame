#pragma once

#include <vector>
#include <unordered_map>
#include <utility>
#include <memory>
#include "custom_pair_hasher.hpp"
#include <glm/glm.hpp>
#include <queue>
#include <mutex>
#include <generic/thread_queue.hpp>
#include <generic/triple.hpp>

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
        std::mutex operator_mutex;
        int radius;
        ntd::ThreadQueue<ntd::Quadlet<int, int, int, Block*>> block_creation_queue;
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

        /**
         * @brief accept and create the next block in the block creation queue
         * 
         */
        void NextInBlockCreationQueue();

        /**
         * @brief get the chunk at r,c
         * 
         * @param r 
         * @param c 
         * @return Chunk* 
         */
        Chunk* operator()(const int r, const int c);
    public:

        /**
         * @brief do some work; accept data from thread queues and work on what is needed.
         * 
         */
        void Work();

        /**
         * @brief queue setting the block at xyz to b*
         * 
         * @param x 
         * @param y 
         * @param z 
         * @param b 
         */
        void QueueBlockCreation(const int x, const int y, const int z, Block* b);

        /**
         * @brief atomically ask for a block property
         * 
         * @param x x
         * @param y y
         * @param z z
         * @param prop the member function pointer
         * @return true 
         * @return false 
         */
        bool AskBlockProperty(const int x, const int y, const int z, bool(Block::* prop)());

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
        void UpdatePlayerVisibleChunks(const glm::vec3& position);

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
         * @brief hint that the chunk at r c needs to be remeshed.
         * 
         * @param r 
         * @param c 
         */
        void HintRemeshing(const int r, const int c);

        /**
         * @brief mutex lock, indicating that chunks inside are being operated on. iterate and apply to each chunk.
         * 
         * @param func the function that takes a Chunk*
         */
        void ForEachMut(void(*func)(Chunk*));
    };
}