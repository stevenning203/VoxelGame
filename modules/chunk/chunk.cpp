#include "chunk.hpp"
#include <block/block.hpp>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Project::Chunk::Chunk() : counter(0) {
    this->data = std::vector<Block*>(CHUNK_SIZE * CHUNK_DEPTH * CHUNK_SIZE, nullptr);
    glGenBuffers(1, &this->vbo_id);
}

Project::Chunk::~Chunk() {
    glDeleteBuffers(1, &this->vbo_id);
}

void Project::Chunk::ReMesh() {
    this->counter = 0;
    for (int r{0}; r < CHUNK_SIZE; r++) {
        for (int c{0}; c < CHUNK_SIZE; c++) {
            for (int y{0}; y < CHUNK_DEPTH; y++) {
                Block* block = this->operator()(r, c, y);
                auto lambda = [&](const int row, const int col, const int depth, unsigned int face){
                    if (row < 0 || col < 0 || depth < 0 ||
                    row >= CHUNK_SIZE || col >= CHUNK_SIZE ||
                    depth >= CHUNK_DEPTH) {
                        return;
                    }
                    
                    if (block->IsOpaque()) {
                        this->mesh[this->counter] = (r * CHUNK_SIZE * CHUNK_DEPTH + y * CHUNK_SIZE + c) | (face << 16);
                        this->counter++;
                    }
                };
                lambda(r + 1, c, y, 2);
                lambda(r - 1, c, y, 4);
                lambda(r, c + 1, y, 5);
                lambda(r, c - 1, y, 0);
                lambda(r, c, y - 1, 3);
                lambda(r, c, y + 1, 1);
            }
        }
    }
}

void Project::Chunk::PushMesh() {
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
    glBufferData(GL_ARRAY_BUFFER, this->counter, &this->mesh[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 1, )
}

Project::Block* Project::Chunk::operator()(const int x, const int y, const int z) {
    return data.at(x * CHUNK_SIZE * CHUNK_DEPTH + y * CHUNK_SIZE + z);
}