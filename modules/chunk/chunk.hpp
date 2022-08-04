#pragma once

#include <vector>
#include <memory>
#include <atomic>
#include <shared_mutex>
#include <item/item.hpp>

namespace Project {
    class Block;

    /**
     * @brief A chunk composed of some set number of blocks.
     * 
     */
    class Chunk {
    private:
        /**
         * @brief generation struct for generating chunks
         * 
         */
        class GenerationStruct {
            
        };

        std::vector<Block*> data;
        unsigned int vbo_id, vao_id;
        std::atomic<bool> empty, chunk_ready, needs_pushing, mesh_ready;
        std::atomic<bool> gl_inited;
        std::shared_mutex mutex;
        std::vector<unsigned int> mesh;
        unsigned short counter;
        unsigned short last_counter;
        unsigned short render_counter; 
        std::atomic<bool> needs_remeshing;
        int row, col;
        Chunk* negative_z;
        Chunk* positive_z;
        Chunk* negative_x;
        Chunk* positive_x;

        /**
         * @brief fill the data with nullptrs
         * 
         */
        void FillNullData();
                
        /**
         * @brief Access the block given at the x y z coordiate a b c
         * NOT THREAD SAFE.
         * 
         * @param x x
         * @param y y
         * @param z z
         * @return Block* the reference to pointer to the block desired.
         */
        inline Block*& operator()(const int x, const int y, const int z);

        /**
         * @brief upload the vertexes to the buffer vbo
         * 
         */
        void PushMeshData();

    public:
        static const int CHUNK_SIZE = 16;
        static const int CHUNK_DEPTH = 128;
        static const int CHUNK_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
        static const int CHUNK_VOLUME = CHUNK_SQUARED * CHUNK_DEPTH;

        /**
         * @brief Construct a new Chunk object with the size of the 3d object given by the static dimensions
         * 
         */
        Chunk(const int row, const int col);

        /**
         * @brief steal a chunks buffers
         * 
         * @param chunk 
         * @param row 
         * @param col 
         */
        Chunk(Chunk* chunk, const int row, const int col);

        /**
         * @brief call gldrawarrays
         * 
         */
        void Render();

        /**
         * @brief world gen the chunk
         * 
         * @param r 
         * @param c 
         */
        void Generate();

        bool IsFinishedGenerating();

        /**
         * @brief delete and replace the block at x y z with b
         * 
         * @param x 
         * @param y 
         * @param z 
         * @param b 
         */
        void RequestReplacement(const int x, const int y, const int z, Block* b);

        /**
         * @brief completely remesh the entire chunk. the generated mesh is stored, but not pushed.
         * 
         * The format of the unsigned int used to store vertex information is as follows:
         * 
         * 0bTTTT_BBBB_BBBB_VVVV_XXXX_ZZZZ_YYYY_YYYY
         * 
         * T -> UV Index
         * 0 -> unused
         * B -> block id numeric
         * X -> x offset
         * Z -> z offest
         * Y -> y offset
         * V -> vertex
         */
        void ReMesh();

        /**
         * @brief atomically suggest that this chunk mesh needs to be remeshed;
         * 
         */
        void SuggestReMesh();

        /**
         * @brief if this mesh needs to be remeshed
         * 
         * @return true 
         * @return false 
         */
        bool NeedsRemeshing();

        /**
         * @brief tell the chunk that it has been remeshed / it no longer needs to be remeshed
         * 
         */
        void ResetNeedsMeshing();

        /**
         * @brief Set the Neighbours objects
         * 
         */
        void SetNeighbours(Chunk* front, Chunk* right, Chunk* bottom, Chunk* left);

        /**
         * @brief ask for a block property
         * 
         * @param x 
         * @param y 
         * @param z 
         * @param prop 
         * @return true 
         * @return false 
         */
        bool AskBlockProperty(const int x, const int y, const int z, bool(Block::* prop)());

        /**
         * @brief ask for a block property
         * 
         * @param x 
         * @param y 
         * @param z 
         * @param prop 
         * @return Item::ToolTypeEnum 
         */
        Item::ToolTypeEnum AskBlockProperty(const int x, const int y, const int z, Item::ToolTypeEnum(Block::* prop)());

        /**
         * @brief ask for a block property
         * 
         * @param x 
         * @param y 
         * @param z 
         * @param prop 
         * @return float 
         */
        float AskBlockProperty(const int x, const int y, const int z, float(Block::* prop)());

        /**
         * @brief return whether or not this mesh is ready for rendering
         * 
         * @return true 
         * @return false 
         */
        bool IsMeshReady();
    };
}