#pragma once

#include <unordered_map>
#include "custom_pair_hasher.hpp"
#include <generic/thread_queue.hpp>
#include <utility>
#include <generic/triple.hpp>
#include <mutex>

namespace Project {
    class ChunkMesh;
    class ChunkManager;
    class Chunk;

    /**
     * @brief collection of chunk meshes
     * 
     */
    class ChunkMeshManager {
        std::unordered_map<std::pair<int, int>, ChunkMesh*, CustomChunkPairHasher> meshes;
        ntd::ThreadQueue<ntd::Triple<int, int, Chunk*>> chunk_meshing_queue;
        ntd::ThreadQueue<std::pair<int, int>> chunk_remeshing_queue;
        ChunkManager* partner;
        std::mutex map_access_mutex;
    public:
        /**
         * @brief Construct a new Chunk Mesh Manager object, partner the two managers
         * 
         * @param p 
         */
        ChunkMeshManager(ChunkManager* p);

        /**
         * @brief suggest that some given chunk needs meshing at the pair {x, z}
         * 
         * @param p 
         */
        void QueueMeshGeneration(int a, int b, Chunk* x);

        /**
         * @brief suggest that some given chunk needs remeshing at the pair {x, z}
         * 
         * @param a 
         * @param b 
         */
        void QueueRemesh(int a, int b);

        /**
         * @brief generate meshes that need to be generated
         * 
         */
        void GenerateQueuedMeshes();

        /**
         * @brief queue all meshes that need remeshing and remesh one mesh from the queue.
         * 
         */
        void ReMeshFlaggedMeshes();

        /**
         * @brief return the STL iterator to the beginning
         * 
         * @return std::unordered_map<std::pair<int, int>, std::unique_ptr<Chunk>, CustomChunkPairHasher>::iterator 
         */
        std::unordered_map<std::pair<int, int>, ChunkMesh*, CustomChunkPairHasher>::iterator begin();

        /**
         * @brief return the STL iterator to the end
         * 
         * @return std::unordered_map<std::pair<int, int>, std::unique_ptr<Chunk>, CustomChunkPairHasher>::iterator 
         */
        std::unordered_map<std::pair<int, int>, ChunkMesh*, CustomChunkPairHasher>::iterator end();
    };
}