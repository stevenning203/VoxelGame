#include "chunk_manager.hpp"
#include "chunk.hpp"
#include <block/grass_block.hpp>
#include <glm/gtc/noise.hpp>
#include <block/air_block.hpp>
#include <block/grass_block.hpp>
#include <block/dirt_block.hpp>
#include <iostream>
#include <block/block.hpp>
#include "chunk_mesh_manager.hpp"
#include "chunk_mesh.hpp"
#include <cmath>

Project::ChunkManager::ChunkManager(ChunkMeshManager* partner) : radius(5), partner(partner) {

}

void Project::ChunkManager::WorldGen() {
    for (int i{-2}; i <= 2; i++) {
        for (int j{-2}; j <= 2; j++) {
            this->chunks[{i, j}] = new Chunk(i, j);
        }
    }
    for (auto& pair : this->chunks) {
        GenerateChunk(pair.second->GetRow(), pair.second->GetCol());
        this->partner->QueueMeshGeneration(pair.second->GetRow(),
        pair.second->GetCol(),
        this->chunks[{pair.second->GetRow(), pair.second->GetCol()}]);
    }
}

Project::Chunk*& Project::ChunkManager::operator()(const int r, const int c) {
    if (!this->chunks.count({r, c})) {
        throw std::runtime_error("chunk manager did not contain");
    }
    return this->chunks[{r, c}];
}

std::unordered_map<std::pair<int, int>, Project::Chunk*, Project::CustomChunkPairHasher>::iterator Project::ChunkManager::begin() {
    return this->chunks.begin();
}

std::unordered_map<std::pair<int, int>, Project::Chunk*, Project::CustomChunkPairHasher>::iterator Project::ChunkManager::end() {
    return this->chunks.end();
}

void Project::ChunkManager::UpdatePlayerVisibleChunks(glm::vec3& position) {
    int left = static_cast<int>(position.x) * 2 / Chunk::CHUNK_SIZE;
    int right = static_cast<int>(position.z) * 2 / Chunk::CHUNK_SIZE; 
    for (int x{-radius + left}; x <= radius + left; x++) {
        int height = static_cast<int>(std::ceil(std::sqrt(radius * radius - (x - left) * (x - left))));
        for (int z{-height + right}; z <= height + right; z++) {
            if (!this->chunks.count({x, z})) {
                this->chunks[{x, z}] = new Chunk(x, z);
                this->chunk_generation_queue.push({x, z});
            }
        }
    }
    NextInChunkQueue();
}

void Project::ChunkManager::NextInChunkQueue() {
    if (this->chunk_generation_queue.empty()) {
        return;
    }
    int x = this->chunk_generation_queue.front().first;
    int z = this->chunk_generation_queue.front().second;
    this->chunk_generation_queue.pop();
    GenerateChunk(x, z);
    this->partner->QueueMeshGeneration(x, z, this->chunks[{x, z}]);
}

void Project::ChunkManager::GenerateChunk(const int row, const int col) {
    Chunk* chunk = this->chunks[{row, col}];
    for (int r{0}; r < Chunk::CHUNK_SIZE; r++) {
        for (int c{0}; c < Chunk::CHUNK_SIZE; c++) {
            int voxel_x = r + chunk->GetRow() * Chunk::CHUNK_SIZE;
            int voxel_z = c + chunk->GetCol() * Chunk::CHUNK_SIZE;
            float noise = glm::simplex(glm::vec2(voxel_x / 20.f, voxel_z / 20.f));
            int h = static_cast<int>(5.f * (noise + 1.f)) + 15;
            for (int y{0}; y < h; y++) {
                chunk->operator()(r, y, c) = new DirtBlock();
            }
            chunk->operator()(r, h, c) = new GrassBlock();
            for (int x{h + 1}; x < Chunk::CHUNK_DEPTH; x++) {
                chunk->operator()(r, x, c) = new AirBlock();
            }
        }
    }
    chunk->SetReady();
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