#pragma once

#include <vector>

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
        unsigned int vbo_id;
        unsigned short counter;

    public:
        static const int CHUNK_SIZE = 16;
        static const int CHUNK_DEPTH = 128;

        /**
         * @brief Construct a new Chunk object with the size of the 3d object given by the static dimensions
         * 
         */
        Chunk();

        /**
         * @brief Destroy the Chunk object and all blocks associated with it;
         * 
         */
        ~Chunk();

        /**
         * @brief completely remesh the entire chunk. the generated mesh is stored, but not pushed.
         * 
         */
        void ReMesh();

        /**
         * @brief Bind the mesh VBO, such that it is ready to be rendered.
         * 
         */
        void PushMesh();
        
        /**
         * @brief Access the block given at the x y z coordiate a b c
         * 
         * @param x x
         * @param y y
         * @param z z
         * @return Block* the reference to the block desired.
         */
        Block* operator()(const int x, const int y, const int z);
    };
}