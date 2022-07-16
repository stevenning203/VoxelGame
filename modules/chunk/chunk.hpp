#pragma once

#include <vector>
#include <memory>
#include <atomic>

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
        int row, col;
        bool empty;
        std::atomic<bool> ready;
        std::atomic<bool> needs_remeshing;

        void FillNullData();

    public:
        static const int CHUNK_SIZE = 16;
        static const int CHUNK_DEPTH = 128;
        static const int CHUNK_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
        static const int CHUNK_VOLUME = CHUNK_SQUARED * CHUNK_DEPTH;

        /**
         * @brief Construct a new Chunk object with the size of the 3d object given by the static dimensions
         * 
         */
        Chunk(int row, int col);

        /**
         * @brief Bind the mesh VAO, such that it is ready to be rendered.
         * 
         */
        void PushMesh();

        int GetRow();

        int GetCol();

        unsigned int GetVAO();

        unsigned int GetVBO();

        /**
         * @brief imply that the chunk is ready for rendering
         * 
         */
        void SetReady();

        /**
         * @brief ask if this chunk is ready to render
         * 
         * @return true 
         * @return false 
         */
        bool GetReady();
        
        /**
         * @brief Access the block given at the x y z coordiate a b c
         * 
         * @param x x
         * @param y y
         * @param z z
         * @return Block* the reference to pointer to the block desired.
         */
        Block*& operator()(const int x, const int y, const int z);

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
    };
}