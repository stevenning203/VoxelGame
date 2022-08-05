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
#include <input/mouse_handler.hpp>
#include <block/stone.hpp>
#include <shader/program.hpp>

Project::ChunkManager::ChunkManager(Player* p, MouseHandler* mouse, Program* shader) : shader(shader), selection_box_first(0), block_breaking_progress(0.f), block_breaking_location{-1, -1, -1}, mouse(mouse), player(p), radius(8) {
    this->ray_caster = new DDACaster();
    this->selection_box_vertices.resize(36);
}

void Project::ChunkManager::MainThreadWork() {
    this->RenderSelectionBox();
}

void Project::ChunkManager::ThreadWork() {
    this->ReMeshQueuedMeshes();
    this->NextInBlockCreationQueue();
    this->UpdatePlayerVisibleChunks();
    this->EnablePlayerBlockDestruction();
    this->EnforcePlayerVoxelCollision();
    this->IncrementPlacingCounter();
}

void Project::ChunkManager::IncrementPlacingCounter() {
    this->block_placing_counter++;
    if (this->block_placing_counter >= BLOCK_PLACING_DELAY) {
        this->block_placing_flag = true;
        this->block_placing_counter = 0;
    }
}

void Project::ChunkManager::EnforcePlayerVoxelCollision() {
    
}

void Project::ChunkManager::RenderSelectionBox() {
    constexpr static unsigned int starter[] = {
        0, 0, 0, 0, 0, 0, 0, 0
    };
    constexpr static unsigned int verts[] = {
        4, 0, 1, 5, 4,
        6, 2, 0, 2, 3,
        1, 3, 7, 5, 7,
        6, 4,

        4, 0, 1, 5, 4,
        6, 2, 0, 2, 3,
        1, 3, 7, 5, 7,
        6, 4, 0, 0,
    };
    if (!selection_box_first) {
        glGenVertexArrays(1, &this->selection_box_vao_id);
        glBindVertexArray(this->selection_box_vao_id);
        glGenBuffers(1, &this->selection_box_vbo_id);
        glBindBuffer(GL_ARRAY_BUFFER, this->selection_box_vbo_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribIPointer(1, 2, GL_UNSIGNED_INT, 0, (void*)0);
        selection_box_first = true;
    }
    if (this->block_breaking_location[1] <= -1) {
        return;
    }
    unsigned int y = this->block_breaking_location[1];
    unsigned int x = ChunkMod(this->block_breaking_location[0]);
    unsigned int z = ChunkMod(this->block_breaking_location[2]);
    unsigned int pos = (x << 12) + (z << 8) + y;
    for (int i{0}; i < 17; i++) {
        this->selection_box_vertices[i * 2] = 0;
        this->selection_box_vertices[i * 2 + 1] = (verts[i] << 16) + pos;
    }
    glBindVertexArray(this->selection_box_vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->selection_box_vbo_id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), &this->selection_box_vertices[0]);
    glm::ivec2 indices = WorldCoordinatesToChunkIndices({this->block_breaking_location[0], this->block_breaking_location[2]});
    this->shader->UniformFloat("chunk_offset_x", static_cast<float>(indices[0] * Chunk::CHUNK_SIZE));
    this->shader->UniformFloat("chunk_offset_z", static_cast<float>(indices[1] * Chunk::CHUNK_SIZE));
    glDepthFunc(GL_ALWAYS);
    glDrawArrays(GL_LINE_STRIP, 0, 17);
    glDepthFunc(GL_LESS);
}

void Project::ChunkManager::ReMeshQueuedMeshes() {
    for (std::pair<const std::pair<int, int>, Chunk*>& pair : this->chunks) {
        Chunk* chunk = pair.second;
        const std::pair<int, int>& indices = pair.first;
        static auto lambda = [this](const std::pair<int, int>& pair) {
            return this->chunks.count(pair) && this->chunks[pair]->IsFinishedGenerating();
        };
        if (chunk->NeedsRemeshing()) {
            if (!lambda({indices.first + 1, indices.second}) || 
                !lambda({indices.first - 1, indices.second}) || 
                !lambda({indices.first, indices.second + 1}) || 
                !lambda({indices.first, indices.second - 1}) ||
                !lambda({indices.first - 1, indices.second - 1}) || 
                !lambda({indices.first + 1, indices.second - 1}) ||
                !lambda({indices.first - 1, indices.second + 1}) || 
                !lambda({indices.first + 1, indices.second + 1})) {
                
            } else {
                chunk->SetNeighbours(this->chunks[{indices.first, indices.second - 1}],
                    this->chunks[{indices.first + 1, indices.second}],
                    this->chunks[{indices.first, indices.second + 1}],
                    this->chunks[{indices.first - 1, indices.second}],
                    this->chunks[{indices.first + 1, indices.second - 1}],
                    this->chunks[{indices.first + 1, indices.second + 1}],
                    this->chunks[{indices.first - 1, indices.second + 1}],
                    this->chunks[{indices.first - 1, indices.second - 1}]);
                this->remeshing_queue.push(pair.first);
                chunk->ResetNeedsMeshing();
            }
        }
    }
    if (this->remeshing_queue.empty()) {
        return;
    }
    std::pair<int, int> front = this->remeshing_queue.front();
    this->chunks[front]->ReMesh();
    this->remeshing_queue.pop();
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
    for (const std::pair<const std::pair<int, int>, Chunk*>& pair : this->chunks) {
        
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

float Project::ChunkManager::AskBlockProperty(const int x, const int y, const int z, float(Block::* prop)()) {
    glm::ivec2 modxz = WorldCoordinatesToChunkCoordinates(x, z);
    glm::ivec2 rowcol = WorldCoordinatesToChunkIndices(x, z);
    return this->operator()(rowcol[0], rowcol[1])->AskBlockProperty(modxz[0], y, modxz[1], prop);
}

void Project::ChunkManager::NextInDeletionQueue() {
    if (this->deletion_queue.empty()) {
        return;
    }
}

void Project::ChunkManager::NextInChunkQueue() {
    if (this->chunk_generation_queue.empty()) {
        return;
    }
    GenerateChunk(this->chunk_generation_queue.front());
    this->chunk_generation_queue.pop();
}

void Project::ChunkManager::GenerateChunk(const int row, const int col) {
    this->chunks[{row, col}]->Generate();
}

void Project::ChunkManager::GenerateChunk(const std::pair<int, int>& pair) {
    this->chunks[pair]->Generate();
}

void Project::ChunkManager::QueueBlockCreation(const int x, const int y, const int z, Block* b) {
    this->block_creation_queue.Emplace(x, y, z, b);
}

bool Project::ChunkManager::BlockExists(const int a, const int b, const int c) {
    if (b < 0) {
        return false;
    }
    glm::ivec2 rowcol = WorldCoordinatesToChunkIndices(a, c);
    std::shared_lock lock(this->mutex);
    if (!this->chunks.count({rowcol[0], rowcol[1]}) || !this->chunks[{rowcol[0], rowcol[1]}]->IsFinishedGenerating()) {
        return false;
    }
    glm::ivec2 modxz = WorldCoordinatesToChunkCoordinates(a, c);
    return !(modxz.x * Chunk::CHUNK_SIZE * Chunk::CHUNK_DEPTH + b * Chunk::CHUNK_SIZE + modxz[1] >= Chunk::CHUNK_VOLUME);
}

Project::Chunk* Project::ChunkManager::operator()(const int r, const int c) {
    if (!this->chunks.count({r, c})) {
        throw std::runtime_error("chunk manager did not contain");
    }
    return this->chunks[{r, c}];
}

bool Project::ChunkManager::AskBlockProperty(const int x, const int y, const int z, bool(Block::* prop)()) {
    glm::ivec2 modxz = WorldCoordinatesToChunkCoordinates(x, z);
    glm::ivec2 rowcol = WorldCoordinatesToChunkIndices(x, z);
    return this->operator()(rowcol[0], rowcol[1])->AskBlockProperty(modxz[0], y, modxz[1], prop);
}

Project::Item::ToolTypeEnum Project::ChunkManager::AskBlockProperty(const int x, const int y, const int z, Item::ToolTypeEnum(Block::* prop)()) {
    glm::ivec2 modxz = WorldCoordinatesToChunkCoordinates(x, z);
    glm::ivec2 rowcol = WorldCoordinatesToChunkIndices(x, z);
    return this->operator()(rowcol[0], rowcol[1])->AskBlockProperty(modxz[0], y, modxz[1], prop);
}

void Project::ChunkManager::EnablePlayerBlockDestruction() {
    bool left = mouse->GetMouseState(MouseHandler::MouseEnum::LMB_HELD);
    bool right = mouse->GetMouseState(MouseHandler::MouseEnum::RMB_HELD);
    int r, c, y;
    int pr, pc, py = -1;
    bool collide = this->ray_caster->Cast(this->player->GetPosition(), this->player->GetDirection(), PLAYER_REACH, *this, r, c, y, pr, pc, py);
    if (!collide) {
        this->block_breaking_progress = 0.f;
        this->block_breaking_location[1] = -1.f;
        return;
    }
    if (py != -1 && right) {
        if (block_placing_flag) {
            this->QueueBlockCreation(pr, py, pc, new Stone());
            this->block_placing_counter = 0;
            this->block_placing_flag = false;
        }
    }
    if (left) {
        if (this->block_breaking_location == glm::ivec3{r, y, c}) {
            if (this->player->InHand() == nullptr) {
                this->block_breaking_progress += 1.f;
            } else {
                this->block_breaking_progress += (this->AskBlockProperty(r, y, c, &Block::RequiredTool) == this->player->InHand()->ToolType()) ? this->player->InHand()->BreakingPower() : 1.f;
            }
        } else {
            this->block_breaking_progress = 0.f;
        }
        if (this->block_breaking_progress >= BLOCK_BREAKING_THRESHOLD * this->AskBlockProperty(r, y, c, &Block::Hardness)) {
            // drop blocks here?
            this->QueueBlockCreation(r, y, c, new AirBlock());
            this->block_breaking_progress = 0.f;
        }
    } else {
        this->block_breaking_progress = 0.f;
    }
    this->block_breaking_location.x = r;
    this->block_breaking_location.y = y;
    this->block_breaking_location.z = c;
}

void Project::ChunkManager::ForEachMut(void(*func)(std::pair<const std::pair<int, int>, Chunk*>&, Program*), Program* shader) {
    //std::shared_lock lock(this->mutex);
    for (std::pair<const std::pair<int, int>, Chunk*>& it : this->chunks) {
        func(it, shader);
    }
}

glm::ivec2 Project::ChunkManager::WorldCoordinatesToChunkCoordinates(const int x, const int z) {
    return {ChunkMod(x), ChunkMod(z)};
}

glm::ivec2 Project::ChunkManager::WorldCoordinatesToChunkCoordinates(glm::ivec2 v) {
    v.x = ChunkMod(v.x);
    v.y = ChunkMod(v.y);
    return v;
}

glm::ivec2 Project::ChunkManager::WorldCoordinatesToChunkIndices(const int x, const int z) {
    return {ChunkFloor(x), ChunkFloor(z)};
}

glm::ivec2 Project::ChunkManager::WorldCoordinatesToChunkIndices(glm::ivec2 v) {
    v.x = ChunkFloor(v.x);
    v.y = ChunkFloor(v.y);
    return v;
}