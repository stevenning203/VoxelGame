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
#include <block/stone.hpp>

Project::Chunk::Chunk(const int row, const int col) : render_counter{0}, last_counter{0}, gl_inited(false), row(row), col(col), chunk_ready(false), mesh_ready(false), counter(0), needs_remeshing(false), needs_pushing(false) {
    this->empty = false;
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

void Project::Chunk::Generate() {
    for (int r{0}; r < Chunk::CHUNK_SIZE; r++) {
        for (int c{0}; c < Chunk::CHUNK_SIZE; c++) {
            int voxel_x = r + row * Chunk::CHUNK_SIZE;
            int voxel_z = c + col * Chunk::CHUNK_SIZE;
            float noise = glm::simplex(glm::vec2(voxel_x / 20.f, voxel_z / 20.f));
            int h = static_cast<int>(5.f * (noise + 1.f)) + 15;
            for (int y{0}; y < h - 5; y++) {
                this->operator()(r, y, c) = new Stone();
            }
            for (int y{h - 5}; y < h; y++) {
                this->operator()(r, y, c) = new DirtBlock();
            }
            this->operator()(r, h, c) = new GrassBlock();
            for (int x{h + 1}; x < Chunk::CHUNK_DEPTH; x++) {
                this->operator()(r, x, c) = new AirBlock();
            }
        }
    }
    this->chunk_ready = true;
    this->SuggestReMesh();
}

void Project::Chunk::RequestReplacement(const int x, const int y, const int z, Block* b) {
    delete this->operator()(x, y, z);
    this->operator()(x, y, z) = b;
    this->SuggestReMesh();
}

bool Project::Chunk::AskBlockProperty(const int x, const int y, const int z, bool(Block::* prop)()) {
    std::shared_lock lock{this->mutex};
    return (*this->operator()(x, y, z).*prop)();
}

Project::Block*& Project::Chunk::operator()(const int x, const int y, const int z) {
    return data.at(x * CHUNK_SIZE * CHUNK_DEPTH + y * CHUNK_SIZE + z);
}

void Project::Chunk::ReMesh() {
    if (!this->chunk_ready) {
        return;
    }
    this->mesh_ready = false;
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
                if (this->operator()(r, y, c)->SkipRender()) {
                    continue;
                }
                unsigned int texture_index_start = 3U * static_cast<unsigned int>(this->operator()(r, y, c)->GetID());
                unsigned int pos = (r << 12) + (y) + (c << 8);
                auto lambda = [&](const int row, const int col, const int depth, unsigned int face){
                    if (row < 0 || col < 0 || depth < 0 ||
                    row >= Chunk::CHUNK_SIZE || col >= Chunk::CHUNK_SIZE ||
                    depth >= Chunk::CHUNK_DEPTH || !this->operator()(row, depth, col)->IsOpaque()) {
                        unsigned int texture_index = texture_index_start;
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
                            int vertex = pos | (vertex_index << 16) | (texture_index << 20) | (uv_index << 28);
                            this->mesh.at(this->counter) = vertex;
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
    this->render_counter = counter;
    this->needs_pushing = true;
    this->mesh_ready = true;
    this->needs_remeshing = false;
}

void Project::Chunk::PushMeshData() {
    if (!this->mesh_ready) {
        return;
    }
    if (!this->gl_inited) {
        glGenVertexArrays(1, &this->vao_id);
        glBindVertexArray(this->vao_id);
        glGenBuffers(1, &this->vbo_id);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
        glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, 0, (void*)0);
        glEnableVertexAttribArray(1);
        this->gl_inited = true;
    }
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
    if (this->counter > this->last_counter) {
        glBufferData(GL_ARRAY_BUFFER, (int)this->counter * sizeof(unsigned int), &this->mesh.at(0), GL_STREAM_DRAW);
    } else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, (int)this->counter * sizeof(unsigned int), &this->mesh.at(0));
    }
}

void Project::Chunk::Render() {
    if (this->render_counter == 0) {
        return;
    }
    if (this->mesh_ready && (this->needs_pushing || !this->gl_inited)) {
        this->PushMeshData();
        this->needs_pushing = false;
    }
    glBindVertexArray(this->vao_id);
    glDrawArrays(GL_TRIANGLES, 0, this->render_counter);
}

void Project::Chunk::SuggestReMesh() {
    this->needs_remeshing = true;
}

bool Project::Chunk::NeedsRemeshing() {
    return this->needs_remeshing;
}

void Project::Chunk::ResetNeedsMeshing() {
    this->needs_remeshing = false;
}

bool Project::Chunk::IsFinishedGenerating() {
    return this->chunk_ready;
}

bool Project::Chunk::IsMeshReady() {
    return this->mesh_ready;
}