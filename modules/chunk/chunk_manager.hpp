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
#include <item/item.hpp>

namespace Project {
    class Chunk;
    class CustomChunkPairHasher;
    class Block;
    class Player;
    class DDACaster;
    class Program;
    class MouseHandler;

    /**
     * @brief Manager for chunks
     * 
     */
    class ChunkManager : public Workable {
        constexpr static float PLAYER_REACH = 5.f;
        constexpr static float BLOCK_BREAKING_THRESHOLD = 10000.f;
        constexpr static int BLOCK_PLACING_DELAY = 100;
        int block_placing_counter = 0;
        bool block_placing_flag = true;
        DDACaster* ray_caster;
        Player* player;
        Program* shader;
        MouseHandler* mouse;
        std::unordered_map<std::pair<int, int>, Chunk*, CustomChunkPairHasher> chunks;
        std::queue<std::pair<int, int>> chunk_generation_queue;
        std::shared_mutex mutex;

        ntd::ThreadQueue<ntd::Quadlet<int, int, int, Block*>> block_creation_queue;
        std::queue<std::pair<int, int>> remeshing_queue;
        std::queue<std::pair<int, int>> deletion_queue;
        int radius;
        unsigned int selection_box_vao_id, selection_box_vbo_id;
        std::vector<unsigned int> selection_box_vertices;
        bool selection_box_first;

        glm::ivec3 block_breaking_location;
        float block_breaking_progress;
        float prev_hardness;

        /**
         * @brief increment the placing counter and flag when a block can be placed
         * 
         */
        void IncrementPlacingCounter();

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
         * @brief world generaation for chunk at pair
         * 
         * @param pair 
         */
        void GenerateChunk(const std::pair<int, int>& pair);

        /**
         * @brief accept the next chunk in the generation queue.
         * 
         */
        void NextInChunkQueue();

        /**
         * @brief accept the next chunk in the deletion
         * 
         */
        void NextInDeletionQueue();

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

        /**
         * @brief convert world coordinates to local chunk coordinates
         * 
         * @param x 
         * @param z 
         * @return glm::ivec2 
         */
        static glm::ivec2 WorldCoordinatesToChunkCoordinates(const int x, const int z);

        /**
         * @brief convert world coordinates to local chunk coordinates
         * 
         * @param v 
         * @return glm::ivec2 
         */
        static glm::ivec2 WorldCoordinatesToChunkCoordinates(glm::ivec2 v);

        /**
         * @brief convert world coordinates to chunk indices for the chunk manager
         * 
         * @param x 
         * @param z 
         * @return glm::ivec2 
         */
        static glm::ivec2 WorldCoordinatesToChunkIndices(const int x, const int z);

        /**
         * @brief convert world coordinates to chunk indices for the chunk manager
         * 
         * @param v 
         * @return glm::ivec2 
         */
        static glm::ivec2 WorldCoordinatesToChunkIndices(glm::ivec2 v);

        /**
         * @brief enforce player and voxel collisions
         * 
         * @param cm 
         * @param p 
         */
        void EnforcePlayerVoxelCollision();

        /**
         * @brief draw a box around the selected block
         * 
         */
        void RenderSelectionBox();
    public:
        virtual void MainThreadWork() override;

        virtual void ThreadWork() override;

        /**
         * @brief ask for a block boolean property
         * 
         * @param x 
         * @param y 
         * @param z 
         * @param Block:: pointer to the block function
         * @return true 
         * @return false 
         */
        bool AskBlockProperty(const int x, const int y, const int z, bool(Block::*)());

        /**
         * @brief ask for a block property...
         * 
         * @param x x
         * @param y y
         * @param z z
         * @param Block:: pointer to the function
         * @return Item::ToolTypeEnum the tool type
         */
        Item::ToolTypeEnum AskBlockProperty(const int x, const int y, const int z, Item::ToolTypeEnum(Block::*)());

        /**
         * @brief ask for a block property...
         * 
         * @param x 
         * @param y 
         * @param z 
         * @param Block:: pointer to the function
         * @return float 
         */
        float AskBlockProperty(const int x, const int y, const int z, float(Block::*)());
        
        /**
         * @brief answer if the block exists
         * 
         * @param x 
         * @param y 
         * @param z 
         * @return true 
         * @return false 
         */
        bool BlockExists(const int x, const int y, const int z);

        /**
         * @brief Construct a new Chunk Manager object
         * 
         */
        ChunkManager(Player* p, MouseHandler* m, Program* shader);

        /**
         * @brief mutex lock, indicating that chunks inside are being operated on. iterate and apply to each chunk.
         * 
         * @param func the function that takes a Chunk*
         */
        void ForEachMut(void(*)(std::pair<const std::pair<int, int>, Chunk*>&, Program*), Program*);
    };
}