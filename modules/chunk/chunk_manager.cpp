#include "chunk_manager.hpp"
#include "chunk.hpp"
#include <block/grass_block.hpp>

void Project::ChunkManager::WorldGen() {
    this->chunks.emplace_back(0, 0);
}

std::vector<Project::Chunk>& Project::ChunkManager::GetChunks() {
    return this->chunks;
}

void Project::ChunkManager::SuggestRemesh() {
    for (Chunk& c : this->chunks) {
        c.ReMesh();
    }
}