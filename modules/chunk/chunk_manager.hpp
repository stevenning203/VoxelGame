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
#include <shared_mutex>
#include <generic/workable.hpp>

namespace Project {
    class Chunk;
    class CustomChunkPairHasher;
    class Block;
    class Player;
    class DDACaster;

    /**
     * @brief Manager for chunks
     * 
     */
    class ChunkManager : public Workable {
        constexpr static float PLAYER_REACH = 5.f;
        DDACaster* ray_caster;
        Player* player;
        std::unordered_map<std::pair<int, int>, Chunk*, CustomChunkPairHasher> chunks;
        std::queue<std::pair<int, int>> chunk_generation_queue;
        std::shared_mutex mutex;

        ntd::ThreadQueue<ntd::Quadlet<int, int, int, Block*>> block_creation_queue;
        ntd::ThreadQueue<std::pair<int, int>> remeshing_queue;

        int radius;

        /**
         * @brief 
         * 
         */
        void ReMeshQueuedMeshes();

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

        /**
         * @brief Remove out of vision chunks and add chunks that should be in viewable distance
         * 
         * @param position the position with which we want to update respect to
         */
        void UpdatePlayerVisibleChunks();

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
         * @brief allow the player to select blocks
         * 
         * @param cm 
         * @param c 
         * @param reach 
         */
        void EnablePlayerBlockDestruction();
    public:
        virtual void MainThreadWork() override;

        virtual void ThreadWork() override;

        bool AskBlockProperty(const int x, const int y, const int z, bool(Block::*)());
        
        bool BlockExists(const int x, const int y, const int z);

        /**
         * @brief Construct a new Chunk Manager object
         * 
         */
        ChunkManager(Player* p);

        /**
         * @brief mutex lock, indicating that chunks inside are being operated on. iterate and apply to each chunk.
         * 
         * @param func the function that takes a Chunk*
         */
        void ForEachMut(const std::function<void(std::pair<const std::pair<int, int>, Chunk*>&)>& func);
    };
}