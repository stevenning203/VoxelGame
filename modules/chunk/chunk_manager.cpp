#include "chunk_manager.hpp"
#include "chunk.hpp"
#include <block/grass_block.hpp>
#include <glm/gtc/noise.hpp>
#include <block/air_block.hpp>
#include <block/grass_block.hpp>
#include <block/dirt_block.hpp>
#include <iostream>
#include <block/block.hpp>
#include <cmath>
#include <generic/mod.hpp>
#include <generic/triple.hpp>
#include <player/player.hpp>
#include <physics/dda_caster.hpp>
#include <physics/world_collision_handler.hpp>

Project::ChunkManager::ChunkManager(Player* p) : player(p), radius(5) {
    this->ray_caster = new DDACaster();
}

void Project::ChunkManager::MainThreadWork() {
    
}

void Project::ChunkManager::ThreadWork() {
    this->ReMeshQueuedMeshes();
    this->NextInBlockCreationQueue();
    this->UpdatePlayerVisibleChunks();
}

void Project::ChunkManager::ReMeshQueuedMeshes() {
    for (std::pair<std::pair<int, int>, Chunk*> pair : this->chunks) {
        Chunk* chunk = pair.second;
        if (chunk->NeedsRemeshing()) {
            this->remeshing_queue.Push(pair.first);
            chunk->ResetNeedsMeshing();
        }
    }
    if (this->remeshing_queue.Empty()) {
        return;
    }
    std::pair<int, int> front = this->remeshing_queue.Front();
    this->chunks[front]->ReMesh();
    this->remeshing_queue.Pop();
}

void Project::ChunkManager::UpdatePlayerVisibleChunks() {
    const glm::vec3& position = this->player->GetPosition();
    std::unique_lock lock{this->mutex};
    int left = static_cast<int>(position.x) / Chunk::CHUNK_SIZE;
    int right = static_cast<int>(position.z) / Chunk::CHUNK_SIZE; 
    for (int x{-radius + left}; x <= radius + left; x++) {
        int height = static_cast<int>(std::ceil(std::sqrt(radius * radius - (x - left) * (x - left))));
        for (int z{-height + right}; z <= height + right; z++) {
            if (!this->chunks.count({x, z})) {
                this->chunks[{x, z}] = new Chunk(x, z);
                this->chunk_generation_queue.push({x, z});
            }
        }
    }
    lock.unlock();
    NextInChunkQueue();
}

void Project::ChunkManager::NextInBlockCreationQueue() {
    if (this->block_creation_queue.Empty()) {
        return;
    }
    ntd::Quadlet<int, int, int, Block*> f = this->block_creation_queue.Front();
    int row = FloorDiv(f.First(), Chunk::CHUNK_SIZE);
    int col = FloorDiv(f.Third(), Chunk::CHUNK_SIZE);
    int modx = Mod(f.First(), Chunk::CHUNK_SIZE);
    int modz = Mod(f.Third(), Chunk::CHUNK_SIZE);
    this->chunks[{row, col}]->RequestReplacement(modx, f.Second(), modz, f.Fourth());
    this->block_creation_queue.Pop();
}

void Project::ChunkManager::NextInChunkQueue() {
    if (this->chunk_generation_queue.empty()) {
        return;
    }
    int x = this->chunk_generation_queue.front().first;
    int z = this->chunk_generation_queue.front().second;
    this->chunk_generation_queue.pop();
    GenerateChunk(x, z);
}

void Project::ChunkManager::GenerateChunk(const int row, const int col) {
    this->chunks[{row, col}]->Generate();
}

void Project::ChunkManager::QueueBlockCreation(const int x, const int y, const int z, Block* b) {
    this->block_creation_queue.Emplace(x, y, z, b);
}

bool Project::ChunkManager::BlockExists(const int a, const int b, const int c) {
    if (b < 0) {
        return false;
    }
    int row = FloorDiv(a, Chunk::CHUNK_SIZE);
    int col = FloorDiv(c, Chunk::CHUNK_SIZE);
    std::shared_lock lock(this->mutex);
    if (!this->chunks.count({row, col}) || !this->chunks[{row, col}]->IsFinishedGenerating()) {
        return false;
    }
    int modx = Mod(a, Chunk::CHUNK_SIZE);
    int modz = Mod(c, Chunk::CHUNK_SIZE);
    return !(modx * Chunk::CHUNK_SIZE * Chunk::CHUNK_DEPTH + b * Chunk::CHUNK_SIZE + modz >= Chunk::CHUNK_VOLUME);
}

Project::Chunk* Project::ChunkManager::operator()(const int r, const int c) {
    if (!this->chunks.count({r, c})) {
        throw std::runtime_error("chunk manager did not contain");
    }
    return this->chunks[{r, c}];
}

bool Project::ChunkManager::AskBlockProperty(const int x, const int y, const int z, bool(Block::* prop)()) {
    std::shared_lock lock(this->mutex);
    int row = FloorDiv(x, Chunk::CHUNK_SIZE);
    int col = FloorDiv(z, Chunk::CHUNK_SIZE);
    int modx = Mod(x, Chunk::CHUNK_SIZE);
    int modz = Mod(z, Chunk::CHUNK_SIZE);
    return this->operator()(row, col)->AskBlockProperty(modx, y, modz, prop);
}

void Project::ChunkManager::EnablePlayerBlockDestruction() {
    int r, c, y;
    int pr, pc, py;
    bool collide = this->ray_caster->Cast(this->player->GetPosition(), this->player->GetDirection(), PLAYER_REACH, *this, r, c, y, pr, pc, py);
    if (!collide) {
        return;
    }
    // drop blocks here?
    this->QueueBlockCreation(r, y, c, new AirBlock());
}

void Project::ChunkManager::ForEachMut(void(*func)(std::pair<const std::pair<int, int>, Chunk*>&, Program*), Program* shader) {
    //std::shared_lock lock(this->mutex);
    for (std::pair<const std::pair<int, int>, Chunk*>& it : this->chunks) {
        func(it, shader);
    }
}