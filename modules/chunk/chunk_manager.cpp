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
#include <generic/mod.hpp>
#include <generic/triple.hpp>

Project::ChunkManager::ChunkManager(ChunkMeshManager* partner) : radius(5), partner(partner) {

}

void Project::ChunkManager::HintRemeshing(const int r, const int c) {
    this->partner->QueueRemesh(r, c);
}

void Project::ChunkManager::WorldGen() {
    for (int i{-2}; i <= 2; i++) {
        for (int j{-2}; j <= 2; j++) {
            this->chunks[{i, j}] = new Chunk();
        }
    }
    for (auto& pair : this->chunks) {
        GenerateChunk(pair.first.first, pair.first.second);
        this->partner->QueueMeshGeneration(pair.first.first,
        pair.first.second,
        this->chunks[{pair.first.first, pair.first.second}]);
    }
}

void Project::ChunkManager::UpdatePlayerVisibleChunks(const glm::vec3& position) {
    int left = static_cast<int>(position.x) / Chunk::CHUNK_SIZE;
    int right = static_cast<int>(position.z) / Chunk::CHUNK_SIZE; 
    for (int x{-radius + left}; x <= radius + left; x++) {
        int height = static_cast<int>(std::ceil(std::sqrt(radius * radius - (x - left) * (x - left))));
        for (int z{-height + right}; z <= height + right; z++) {
            if (!this->chunks.count({x, z})) {
                this->operator_mutex.lock();
                this->chunks[{x, z}] = new Chunk();
                this->operator_mutex.unlock();
                this->chunk_generation_queue.push({x, z});
            }
        }
    }
    NextInChunkQueue();
}

void Project::ChunkManager::NextInBlockCreationQueue() {
    if (this->block_creation_queue.Empty()) {
        return;
    }
    std::lock_guard<std::mutex> lock(this->operator_mutex);
    ntd::Quadlet<int, int, int, Block*> f = this->block_creation_queue.Front();
    int row = FloorDiv(f.First(), Chunk::CHUNK_SIZE);
    int col = FloorDiv(f.Third(), Chunk::CHUNK_SIZE);
    int modx = Mod(f.First(), Chunk::CHUNK_SIZE);
    int modz = Mod(f.Third(), Chunk::CHUNK_SIZE);
    this->chunks[{row, col}]->RequestReplacement(modx, f.Second(), modz, f.Fourth());
    this->block_creation_queue.Pop();
}

void Project::ChunkManager::Work() {
    this->NextInBlockCreationQueue();
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
    this->chunks[{row, col}]->Generate(row, col);
}

void Project::ChunkManager::QueueBlockCreation(const int x, const int y, const int z, Block* b) {
    this->block_creation_queue.Emplace(x, y, z, b);
}

bool Project::ChunkManager::BlockExists(const int a, const int b, const int c) {
    std::lock_guard<std::mutex> lock(this->operator_mutex);
    if (b < 0) {
        return false;
    }
    int row = FloorDiv(a, Chunk::CHUNK_SIZE);
    int col = FloorDiv(c, Chunk::CHUNK_SIZE);
    if (!this->chunks.count({row, col})) {
        return false;
    }
    int modx = Mod(a, Chunk::CHUNK_SIZE);
    int modz = Mod(c, Chunk::CHUNK_SIZE);
    return this->chunks[{row, col}]->Contains(modx, b, modz);
}

Project::Chunk* Project::ChunkManager::operator()(const int r, const int c) {
    if (!this->chunks.count({r, c})) {
        throw std::runtime_error("chunk manager did not contain");
    }
    return this->chunks[{r, c}];
}

bool Project::ChunkManager::AskBlockProperty(const int x, const int y, const int z, bool(Block::* prop)()) {
    std::lock_guard<std::mutex> lock(this->operator_mutex);
    int row = FloorDiv(x, Chunk::CHUNK_SIZE);
    int col = FloorDiv(z, Chunk::CHUNK_SIZE);
    int modx = Mod(x, Chunk::CHUNK_SIZE);
    int modz = Mod(z, Chunk::CHUNK_SIZE);
    return this->operator()(row, col)->AskBlockProperty(x, y, z, prop);
}