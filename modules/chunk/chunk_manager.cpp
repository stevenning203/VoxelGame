#include "chunk_manager.hpp"
#include "chunk.hpp"
#include <block/grass_block.hpp>
#include <glm/gtc/noise.hpp>
#include <block/air_block.hpp>
#include <block/grass_block.hpp>
#include <iostream>
#include <block/block.hpp>

void Project::ChunkManager::WorldGen() {
    for (int i{-2}; i <= 2; i++) {
        for (int j{-2}; j <= 2; j++) {
            this->chunks[{i, j}] = new Chunk(i, j);
        }
    }
    for (auto& pair : this->chunks) {
        for (int r{0}; r < Chunk::CHUNK_SIZE; r++) {
            for (int c{0}; c < Chunk::CHUNK_SIZE; c++) {
                int voxel_x = r + pair.second->GetRow() * Chunk::CHUNK_SIZE;
                int voxel_z = c + pair.second->GetCol() * Chunk::CHUNK_SIZE;
                float noise = glm::simplex(glm::vec2(voxel_x / 32.f, voxel_z / 32.f));
                int h = static_cast<int>(4.f * (noise + 1.f)) + 15;
                for (int y{0}; y < h; y++) {
                    pair.second->operator()(r, y, c) = new GrassBlock();
                }
                for (int x{h}; x < Chunk::CHUNK_DEPTH; x++) {
                    pair.second->operator()(r, x, c) = new AirBlock();
                }
            }
        }
    }
}

void Project::ChunkManager::SuggestRemesh() {
    for (auto& p : this->chunks) {
        p.second->ReMesh();
    }
}

std::unordered_map<std::pair<int, int>, Project::Chunk*, Project::CustomChunkPairHasher>::iterator Project::ChunkManager::begin() {
    return this->chunks.begin();
}

std::unordered_map<std::pair<int, int>, Project::Chunk*, Project::CustomChunkPairHasher>::iterator Project::ChunkManager::end() {
    return this->chunks.end();
}

Project::Block*& Project::ChunkManager::operator()(const int x, const int y, const int z) {
    int row = x / Chunk::CHUNK_SIZE;
    int col = z / Chunk::CHUNK_SIZE;
    if (!this->chunks.count({row, col})) {
        throw new std::runtime_error("Chunk not loaded when operator() called");
    }
    Chunk* chunk = this->chunks[{row, col}];
    int modx = x % Chunk::CHUNK_SIZE;
    int modz = z % Chunk::CHUNK_SIZE;
    return chunk->operator()(modx, y, modz);
} 