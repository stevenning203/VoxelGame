#include "chunk_mesh.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <block/block.hpp>
#include <chunk/chunk.hpp>

Project::ChunkMesh::ChunkMesh(Chunk* c) : counter(0), chunk(c), needs_remeshing(false), ready(false) {
    this->mesh = std::vector<unsigned int>();
    glGenVertexArrays(1, &this->vao_id);
    glBindVertexArray(this->vao_id);
    glGenBuffers(1, &this->vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
    glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, 0, (void*)0);
    glEnableVertexAttribArray(1);
    this->SuggestReMesh();
}

void Project::ChunkMesh::ReMesh() {
    this->ready = false;
    static const std::vector<std::vector<unsigned int>> indices = {
        { 0, 2, 1, 2, 3, 1 }, // 0
        { 0, 5, 4, 0, 1, 5 }, // 1
        { 1, 3, 7, 1, 7, 5 }, // 2
        { 2, 7, 3, 2, 6, 7 }, // 3
        { 0, 6, 2, 0, 4, 6 }, // 4
        { 4, 5, 6, 6, 5, 7 }  // 5
    };
    this->counter = 0;
    for (int r{0}; r < Chunk::CHUNK_SIZE; r++) {
        for (int c{0}; c < Chunk::CHUNK_SIZE; c++) {
            for (int y{0}; y < Chunk::CHUNK_DEPTH; y++) {
                if (this->chunk->operator()(r, y, c)->SkipRender()) {
                    continue;
                }
                auto lambda = [&](const int row, const int col, const int depth, unsigned int face){
                    if (row < 0 || col < 0 || depth < 0 ||
                    row >= Chunk::CHUNK_SIZE || col >= Chunk::CHUNK_SIZE ||
                    depth >= Chunk::CHUNK_DEPTH || !this->chunk->operator()(row, depth, col)->IsOpaque()) {
                        unsigned int pos = (r << 12) + (y) + (c << 8);
                        unsigned int texture_index = static_cast<unsigned int>(this->chunk->operator()(r, y, c)->GetID());
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
    this->ready = true;
    this->needs_remeshing = false;
}

void Project::ChunkMesh::PushMeshData() {
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
    glBufferData(GL_ARRAY_BUFFER, (int)this->counter * sizeof(unsigned int), &this->mesh.at(0), GL_DYNAMIC_DRAW);
}

unsigned int Project::ChunkMesh::GetVAOID() {
    return this->vao_id;
}

unsigned short Project::ChunkMesh::GetCounter() {
    return this->counter;
}

void Project::ChunkMesh::SuggestReMesh() {
    this->needs_remeshing = true;
}

bool Project::ChunkMesh::NeedsRemeshing() {
    return this->needs_remeshing;
}

bool Project::ChunkMesh::GetReady() {
    return this->ready;
}