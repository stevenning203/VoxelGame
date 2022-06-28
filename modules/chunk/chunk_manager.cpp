#include "chunk_manager.hpp"
#include "chunk.hpp"
#include <block/grass_block.hpp>

void Project::ChunkManager::WorldGen() {
    for (int i{-2}; i <= 2; i++) {
        for (int j{-2}; j <= 2; j++) {
            this->chunks.emplace_back(i, j);
        }
    }
}

std::vector<Project::Chunk>& Project::ChunkManager::GetChunks() {
    return this->chunks;
}

void Project::ChunkManager::SuggestRemesh() {
    for (Chunk& c : this->chunks) {
        c.ReMesh();
    }
}