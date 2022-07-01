#pragma once

#include <vector>
#include <memory>

namespace Project {
    class Block;
    /**
     * @brief A chunk composed of some set number of blocks.
     * 
     */
    class Chunk {
    private:
        std::vector<Block*> data;
        std::vector<unsigned int> mesh;
        unsigned int vbo_id, vao_id;
        unsigned short counter;
        int row, col;

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
         * @brief Destroy the Chunk object and all blocks associated with it;
         * 
         */
        ~Chunk();

        /**
         * @brief Push the data into the chunk buffer; should only be called if something changed and remesh was done.
         * 
         */
        void PushMeshData();

        /**
         * @brief completely remesh the entire chunk. the generated mesh is stored, but not pushed.
         * 
         * The format of the unsigned int used to store vertex information is as follows:
         * 
         * 0b0000_00000_BBBB_VVVV_XXXX_ZZZZ_YYYY_YYYY
         * 
         * 0 -> unused
         * B -> block id numeric
         * X -> x offset
         * Z -> z offest
         * Y -> y offset
         * V -> vertex
         */
        void ReMesh();

        /**
         * @brief Bind the mesh VAO, such that it is ready to be rendered.
         * 
         */
        void PushMesh();

        int GetRow();

        int GetCol();

        unsigned int GetVAO();

        unsigned int GetVBO();

        unsigned short GetCounter();
        
        /**
         * @brief Access the block given at the x y z coordiate a b c
         * 
         * @param x x
         * @param y y
         * @param z z
         * @return Block* the reference to pointer to the block desired.
         */
        Block*& operator()(const int x, const int y, const int z);
    };
}