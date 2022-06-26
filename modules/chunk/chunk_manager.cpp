#include "chunk_manager.hpp"
#include "chunk.hpp"
#include <block/grass_block.hpp>

void Project::ChunkManager::WorldGen() {
    Chunk f;
    this->chunks.push_back(f);
    f(0, 0, 0) = new GrassBlock();
}

std::vector<Project::Chunk>& Project::ChunkManager::GetChunks() {
    return this->chunks;
}