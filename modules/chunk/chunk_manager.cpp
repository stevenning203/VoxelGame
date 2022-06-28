#include "chunk_manager.hpp"
#include "chunk.hpp"
#include <block/grass_block.hpp>
#include <glm/gtc/noise.hpp>
#include <block/air_block.hpp>
#include <block/grass_block.hpp>
#include <iostream>

void Project::ChunkManager::WorldGen() {
    for (int i{-2}; i <= 2; i++) {
        for (int j{-2}; j <= 2; j++) {
            this->chunks.emplace_back(i, j);
        }
    }
    for (Chunk& chunk : this->chunks) {
        for (int r{0}; r < Chunk::CHUNK_SIZE; r++) {
            for (int c{0}; c < Chunk::CHUNK_SIZE; c++) {
                int voxel_x = r + chunk.GetRow() * Chunk::CHUNK_SIZE;
                int voxel_z = c + chunk.GetCol() * Chunk::CHUNK_SIZE;
                float noise = glm::simplex(glm::vec2(voxel_x / 32.f, voxel_z / 32.f));
                int h = static_cast<int>(4.f * (noise + 1.f)) + 15;
                for (int y{0}; y < h; y++) {
                    chunk(r, y, c) = new GrassBlock();
                }
                for (int x{h}; x < Chunk::CHUNK_DEPTH; x++) {
                    chunk(r, x, c) = new AirBlock();
                }
            }
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