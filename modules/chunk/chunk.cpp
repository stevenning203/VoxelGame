#include "chunk.hpp"
#include <block/block.hpp>
#include <block/grass_block.hpp>
#include <block/air_block.hpp>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <mutex>
#include <glm/glm.hpp>
#include <block/dirt_block.hpp>
#include <glm/gtc/noise.hpp>

Project::Chunk::Chunk() {
    this->empty = false;
    this->data = std::vector<Block*>();
    FillNullData();
}

void Project::Chunk::FillNullData() {
    std::scoped_lock lock{this->mutex};
    for (int i{0}; i < CHUNK_VOLUME; i++) {
        this->data.push_back(nullptr);
    }
    if (this->data.size() > CHUNK_VOLUME) {
        throw std::runtime_error("too much block nullptrs in FillNullData");
    }
}

void Project::Chunk::Generate(const int row, const int col) {
    std::scoped_lock lock{this->mutex};
    for (int r{0}; r < Chunk::CHUNK_SIZE; r++) {
        for (int c{0}; c < Chunk::CHUNK_SIZE; c++) {
            int voxel_x = r + row * Chunk::CHUNK_SIZE;
            int voxel_z = c + col * Chunk::CHUNK_SIZE;
            float noise = glm::simplex(glm::vec2(voxel_x / 20.f, voxel_z / 20.f));
            int h = static_cast<int>(5.f * (noise + 1.f)) + 15;
            for (int y{0}; y < h; y++) {
                this->operator()(r, y, c) = new DirtBlock();
            }
            this->operator()(r, h, c) = new GrassBlock();
            for (int x{h + 1}; x < Chunk::CHUNK_DEPTH; x++) {
                this->operator()(r, x, c) = new AirBlock();
            }
        }
    }
}

void Project::Chunk::RequestReplacement(const int x, const int y, const int z, Block* b) {
    std::scoped_lock lock{this->mutex};
    delete this->operator()(x, y, z);
    this->operator()(x, y, z) = b;
}

unsigned int Project::Chunk::GetVAO() {
    return this->vao_id;
}

unsigned int Project::Chunk::GetVBO() {
    return this->vbo_id;
}

bool Project::Chunk::Contains(const int x, const int y, const int z) {
    return !(x * CHUNK_SIZE * CHUNK_DEPTH + y * CHUNK_SIZE + z >= CHUNK_VOLUME);
}

bool Project::Chunk::AskBlockProperty(const int x, const int y, const int z, bool(Block::* prop)()) {
    std::shared_lock lock{this->mutex};
    return (*this->operator()(x, y, z).*prop)();
}

int Project::Chunk::AskBlockID(const int x, const int y, const int z) {
    std::shared_lock lock{this->mutex};
    return this->operator()(x, y, z)->GetID();
}

Project::Block*& Project::Chunk::operator()(const int x, const int y, const int z) {
    if (this->empty) {
        this->empty = false;
        FillNullData();
    }
    return data.at(x * CHUNK_SIZE * CHUNK_DEPTH + y * CHUNK_SIZE + z);
}