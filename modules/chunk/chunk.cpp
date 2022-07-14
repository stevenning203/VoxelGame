#include "chunk.hpp"
#include <block/block.hpp>
#include <block/grass_block.hpp>
#include <block/air_block.hpp>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <mutex>

Project::Chunk::Chunk(int row, int col) : row(row), col(col), empty(true), ready(false) {
    this->data = std::vector<Block*>();
}

void Project::Chunk::SetReady() {
    this->ready = true;
}

bool Project::Chunk::GetReady() {
    return this->ready;
}

void Project::Chunk::FillNullData() {
    for (int i{0}; i < CHUNK_VOLUME; i++) {
        this->data.push_back(nullptr);
    }
    if (this->data.size() > CHUNK_VOLUME) {
        throw std::runtime_error("too much block nullptrs in FillNullData");
    }
}

unsigned int Project::Chunk::GetVAO() {
    return this->vao_id;
}

unsigned int Project::Chunk::GetVBO() {
    return this->vbo_id;
}

Project::Block*& Project::Chunk::operator()(const int x, const int y, const int z) {
    if (empty) {
        empty = false;
        FillNullData();
    }
    return data.at(x * CHUNK_SIZE * CHUNK_DEPTH + y * CHUNK_SIZE + z);
}

int Project::Chunk::GetCol() {
    return this->col;
}

int Project::Chunk::GetRow() {
    return this->row;
}