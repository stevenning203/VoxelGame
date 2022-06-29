#pragma once

#include <vector>
#include <unordered_map>
#include <utility>
#include <memory>
#include "custom_pair_hasher.hpp"

namespace Project {
    class Chunk;
    class CustomChunkPairHasher;

    /**
     * @brief Manager for chunks
     * 
     */
    class ChunkManager {
        std::unordered_map<std::pair<int, int>, Chunk*, CustomChunkPairHasher> chunks;

    public:
        /**
         * @brief Generate the world for some chunks near spawn
         * 
         */
        void WorldGen();

        /**
         * @brief Suggest a remesh on all the visible chunks
         * 
         */
        void SuggestRemesh();

        /**
         * @brief return the STL iterator to the beginning
         * 
         * @return std::unordered_map<std::pair<int, int>, std::unique_ptr<Chunk>, CustomChunkPairHasher>::iterator 
         */
        std::unordered_map<std::pair<int, int>, Chunk*, CustomChunkPairHasher>::iterator begin();

        /**
         * @brief return the STL iterator to the end
         * 
         * @return std::unordered_map<std::pair<int, int>, std::unique_ptr<Chunk>, CustomChunkPairHasher>::iterator 
         */
        std::unordered_map<std::pair<int, int>, Chunk*, CustomChunkPairHasher>::iterator end();
    };
}