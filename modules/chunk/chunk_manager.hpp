#pragma once

#include <vector>
#include <unordered_map>
#include <utility>
#include <memory>
#include "custom_pair_hasher.hpp"

namespace Project {
    class Chunk;
    class CustomChunkPairHasher;
    class Block;

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
         * @brief get the block at the specified WORLD coordinates.
         * 
         * @param x the x world coordinate
         * @param y the y world coordinate
         * @param z the z world coordinate
         * @return Block*& the reference to the block* at the specified location
         */
        Block*& operator()(const int x, const int y, const int z);

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