#include "chunk.hpp"
#include <block/block.hpp>
#include <block/grass_block.hpp>
#include <block/air_block.hpp>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

Project::Chunk::Chunk(int row, int col) : counter(0), row(row), col(col) {
    this->data = std::vector<Block*>();
    for (int i{0}; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_DEPTH; i++) {
        this->data.push_back(nullptr);
    }
    this->mesh = std::vector<unsigned int>();
    glGenVertexArrays(1, &this->vao_id);
    glBindVertexArray(this->vao_id);
    glGenBuffers(1, &this->vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
    glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, 0, (void*)0);
    glEnableVertexAttribArray(1);
}

Project::Chunk::~Chunk() {
    glDeleteVertexArrays(1, &this->vao_id);
    glDeleteBuffers(1, &this->vbo_id);
}

void Project::Chunk::ReMesh() {
    static const std::vector<std::vector<unsigned int>> indices = {
        { 0, 2, 1, 2, 3, 1 }, // 0
        { 0, 5, 4, 0, 1, 5 }, // 1
        { 1, 3, 7, 1, 7, 5 }, // 2
        { 2, 7, 3, 2, 6, 7 }, // 3
        { 0, 6, 2, 0, 4, 6 }, // 4
        { 4, 5, 6, 6, 5, 7 }  // 5
    };
    this->counter = 0;
    for (int r{0}; r < CHUNK_SIZE; r++) {
        for (int c{0}; c < CHUNK_SIZE; c++) {
            for (int y{0}; y < CHUNK_DEPTH; y++) {
                if (this->operator()(r, y, c)->SkipRender()) {
                    continue;
                }
                auto lambda = [&](const int row, const int col, const int depth, unsigned int face){
                    if (row < 0 || col < 0 || depth < 0 ||
                    row >= CHUNK_SIZE || col >= CHUNK_SIZE ||
                    depth >= CHUNK_DEPTH || !this->operator()(row, depth, col)->IsOpaque()) {
                        unsigned int pos = (r << 12) + (y) + (c << 8);
                        unsigned int texture_index = static_cast<unsigned int>(this->operator()(r, y, c)->GetID());
                        texture_index *= 3U;
                        if (face == 3U) {
                            texture_index += 2U;
                        } else if (face != 1U) {
                            texture_index += 1U;
                        }
                        if (texture_index >= 256U) {
                            throw new std::runtime_error
                            ("The texture index is too large for bitwise manipulation. Fix by adding another integer in the vertex information");
                        }
                        for (int i = 0; i < 6; i++) {
                            unsigned int uv_index = 0;
                            unsigned int vertex_index = indices.at(face).at(i);
                            if (face == 0U) {
                                if (vertex_index == 0U) {
                                    uv_index = 0;
                                } else if (vertex_index == 1U) {
                                    uv_index = 1U;
                                } else if (vertex_index == 2U) {
                                    uv_index = 2U;
                                } else if (vertex_index == 3U) {
                                    uv_index = 3U;
                                }
                            } else if (face == 1U) {
                                if (vertex_index == 0U) {
                                    uv_index = 2U;
                                } else if (vertex_index == 5U) {
                                    uv_index = 1U;
                                } else if (vertex_index == 4U) {
                                    uv_index = 0U;
                                } else if (vertex_index == 1U) {
                                    uv_index = 3U;
                                }
                            } else if (face == 2U) {
                                if (vertex_index == 1U) {
                                    uv_index = 0U;
                                } else if (vertex_index == 3U) {
                                    uv_index = 2U;
                                } else if (vertex_index == 7U) {
                                    uv_index = 3U;
                                } else if (vertex_index == 5U) {
                                    uv_index = 1U;
                                }
                            } else if (face == 3U) {
                                if (vertex_index == 2U) {
                                    uv_index = 3U;
                                } else if (vertex_index == 3U) {
                                    uv_index = 2U;
                                } else if (vertex_index == 7U) {
                                    uv_index = 0U;
                                } else if (vertex_index == 6U) {
                                    uv_index = 1U;
                                }
                            } else if (face == 4U) {
                                if (vertex_index == 0U) {
                                    uv_index = 1U;
                                } else if (vertex_index == 6U) {
                                    uv_index = 2U;
                                } else if (vertex_index == 2U) {
                                    uv_index = 3U;
                                } else if (vertex_index == 4U) {
                                    uv_index = 0U;
                                }
                            } else if (face == 5U) {
                                if (vertex_index == 4U) {
                                    uv_index = 1U;
                                } else if (vertex_index == 6U) {
                                    uv_index = 3U;
                                } else if (vertex_index == 5U) {
                                    uv_index = 0U;
                                } else if (vertex_index == 7U) {
                                    uv_index = 2U;
                                }
                            }
                            if (this->counter >= this->mesh.size()) {
                                this->mesh.push_back(0);
                            }
                            this->mesh.at(this->counter) = pos | (vertex_index << 16) | (texture_index << 20) | (uv_index << 28);
                            this->counter++;
                        }
                    }
                };
                lambda(r + 1, c, y, 2);
                lambda(r - 1, c, y, 4);
                lambda(r, c + 1, y, 0);
                lambda(r, c - 1, y, 5);
                lambda(r, c, y - 1, 3);
                lambda(r, c, y + 1, 1);
            }
        }
    }
    this->PushMeshData();
}

unsigned int Project::Chunk::GetVAO() {
    return this->vao_id;
}

unsigned int Project::Chunk::GetVBO() {
    return this->vbo_id;
}

void Project::Chunk::PushMesh() {
    glBindVertexArray(this->vao_id);
}

void Project::Chunk::PushMeshData() {
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
    glBufferData(GL_ARRAY_BUFFER, (int)this->counter * sizeof(unsigned int), &this->mesh.at(0), GL_DYNAMIC_DRAW);
}

Project::Block*& Project::Chunk::operator()(const int x, const int y, const int z) {
    return data.at(x * CHUNK_SIZE * CHUNK_DEPTH + y * CHUNK_SIZE + z);
}

int Project::Chunk::GetCol() {
    return this->col;
}

int Project::Chunk::GetRow() {
    return this->row;
}

unsigned short Project::Chunk::GetCounter() {
    return this->counter;
}