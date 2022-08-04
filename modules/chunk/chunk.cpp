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
#include <item/item.hpp>

Project::Item::ToolTypeEnum Project::Chunk::AskBlockProperty(const int x, const int y, const int z, Item::ToolTypeEnum(Block::* prop)()) {
    std::shared_lock lock{this->mutex};
    return (*this->operator()(x, y, z).*prop)();
}

float Project::Chunk::AskBlockProperty(const int x, const int y, const int z, float(Block::* prop)()) {
    std::shared_lock lock{this->mutex};
    return (*this->operator()(x, y, z).*prop)();
}

Project::Chunk::Chunk(const int row, const int col) : render_counter{0}, last_counter{0}, gl_inited(false), row(row), col(col), chunk_ready(false), mesh_ready(false), counter(0), needs_remeshing(false), needs_pushing(false) {
    this->empty = false;
    FillNullData();
}

Project::Chunk::Chunk(Chunk* chunk, const int row, const int col) {
    this->data.swap(chunk->data);
    this->empty = false;
    this->vao_id = chunk->vao_id;
    this->vbo_id = chunk->vbo_id;
    this->gl_inited = true;
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
            float noise0 = glm::simplex(glm::vec2(voxel_x / 20.f, voxel_z / 20.f));
            float noise1 = glm::perlin(glm::vec2(voxel_x / 18.f, voxel_z / 18.f));
            float noise = noise0 + noise1;
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
    if (x < 0) {
        return this->negative_x->operator()(x + CHUNK_SIZE, y, z);
    }
    if (z < 0) {
        return this->negative_z->operator()(x, y, z + CHUNK_SIZE);
    }
    if (x >= 16) {
        return this->positive_x->operator()(x - CHUNK_SIZE, y, z);
    }
    if (z >= 16) {
        return this->positive_z->operator()(x, y, z - CHUNK_SIZE);
    }
    return data.at(x * CHUNK_SIZE * CHUNK_DEPTH + y * CHUNK_SIZE + z);
}

void Project::Chunk::SetNeighbours(Chunk* top, Chunk* right, Chunk* bottom, Chunk* left) {
    this->negative_z = top;
    this->positive_z = bottom;
    this->positive_x = right;
    this->negative_x = left;
}

void Project::Chunk::ReMesh() {
    if (!this->chunk_ready) {
        return;
    }
    this->mesh_ready = false;
    static const unsigned int indices[6][6] = {
        { 0, 2, 1, 2, 3, 1 }, // 0
        { 0, 5, 4, 0, 1, 5 }, // 1
        { 1, 3, 7, 1, 7, 5 }, // 2
        { 2, 7, 3, 2, 6, 7 }, // 3
        { 0, 6, 2, 0, 4, 6 }, // 4
        { 4, 5, 6, 6, 5, 7 }  // 5
    };
    static constexpr unsigned int uv_index_lookup[6][8] = {
        { 0, 1, 2, 3, 0, 0, 0, 0 },
        { 2, 3, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 2, 0, 1, 0, 3 },
        { 0, 0, 3, 2, 0, 0, 1, 0 },
        { 1, 0, 3, 0, 0, 0, 2, 0 },
        { 0, 0, 0, 0, 1, 0, 3, 2 }
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
                    if (depth < 0 || depth >= CHUNK_DEPTH || !this->operator()(row, depth, col)->IsOpaque()) {
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
                            unsigned int vertex_index = indices[face][i];
                            unsigned int uv_index = uv_index_lookup[face][vertex_index];
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