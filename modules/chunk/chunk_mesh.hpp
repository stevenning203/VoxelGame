#pragma once
#include <vector>
#include <atomic>
#include <generic/idable.hpp>

namespace Project {
    class Chunk;

    /**
     * @brief the mesh for a chunk
     * 
     */
    class ChunkMesh {
        unsigned int vao_id, vbo_id;
        std::vector<unsigned int> mesh;
        unsigned short counter;
        Chunk* chunk;
        std::atomic<bool> needs_remeshing, ready;
    public:
        /**
         * @brief Construct the object and also create a VAO with a VBO to store the mesh data
         * 
         */
        ChunkMesh(Chunk* c);

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
         * @brief upload the vertexes to the buffer vbo
         * 
         */
        void PushMeshData();

        unsigned int GetVAOID();

        unsigned short GetCounter();

        /**
         * @brief return whether or not this mesh is ready for rendering
         * 
         * @return true 
         * @return false 
         */
        bool GetReady();
    };
}