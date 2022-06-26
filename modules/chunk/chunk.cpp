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
    static const int indices[6][6] = {
        { 0, 2, 1, 2, 3, 1 },
        { 0, 5, 4, 0, 1, 5 },
        { 1, 3, 7, 1, 7, 5 },
        { 2, 7, 3, 2, 6, 7 },
        { 0, 6, 2, 0, 4, 6 },
        { 4, 5, 6, 6, 5, 7 }
    };
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
                        int pos = r << 12 + y + c << 8;
                        for (int i = 0; i < 6; i++) {
                            this->mesh[this->counter] = pos | (indices[face][i] << 16);
                            this->counter++;
                        }
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
    glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, sizeof(int), (void*)0);
}

Project::Block*& Project::Chunk::operator()(const int x, const int y, const int z) {
    return data.at(x * CHUNK_SIZE * CHUNK_DEPTH + y * CHUNK_SIZE + z);
}