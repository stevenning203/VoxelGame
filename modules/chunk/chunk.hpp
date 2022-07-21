#pragma once

#include <vector>
#include <memory>
#include <atomic>
#include <mutex>

namespace Project {
    class Block;
    /**
     * @brief A chunk composed of some set number of blocks.
     * 
     */
    class Chunk {
    private:
        std::vector<Block*> data;
        unsigned int vbo_id, vao_id;
        bool empty;
        std::mutex access_lock;

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
        Block*& operator()(const int x, const int y, const int z);

    public:
        static const int CHUNK_SIZE = 16;
        static const int CHUNK_DEPTH = 128;
        static const int CHUNK_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
        static const int CHUNK_VOLUME = CHUNK_SQUARED * CHUNK_DEPTH;

        /**
         * @brief Construct a new Chunk object with the size of the 3d object given by the static dimensions
         * 
         */
        Chunk();

        /**
         * @brief world gen the chunk
         * 
         * @param r 
         * @param c 
         */
        void Generate(const int r, const int c);

        /**
         * @brief Bind the mesh VAO, such that it is ready to be rendered.
         * 
         */
        void PushMesh();

        unsigned int GetVAO();

        unsigned int GetVBO();

        /**
         * @brief true if the block is contained here.
         * 
         * @param x 
         * @param y 
         * @param z 
         * @return true 
         * @return false 
         */
        bool Contains(const int x, const int y, const int z);

        /**
         * @brief atomically ask for a boolean property of a block
         * 
         * @param x 
         * @param y 
         * @param z 
         * @param prop func
         * @return true 
         * @return false 
         */
        bool AskBlockProperty(const int x, const int y, const int z, bool(Block::* prop)());

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
         * @brief atomically ask for the ID of a block
         * 
         * @param x 
         * @param y 
         * @param z 
         * @return int 
         */
        int AskBlockID(const int x, const int y, const int z);
    };
}