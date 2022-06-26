#include "chunk.hpp"
#include <block/block.hpp>
#include <vector>

Project::Chunk::Chunk() {
    this->data = std::vector<Block*>(CHUNK_SIZE * CHUNK_DEPTH * CHUNK_SIZE, nullptr);
}

Project::Chunk::~Chunk() {
    
}

Project::Block*& Project::Chunk::operator()(const int r, const int c, const int w) {
    return data[r * CHUNK_SIZE * CHUNK_SIZE + c * CHUNK_SIZE + w];
}