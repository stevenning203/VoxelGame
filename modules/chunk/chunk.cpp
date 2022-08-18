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

Project::Item::ToolTypeEnum Project::Chunk::AskBlockProperty(const int x, const int y, const int z, Item::ToolTypeEnum(Block::* prop)() const) const {
    std::shared_lock lock{this->mutex};
    return (*this->operator()(x, y, z).*prop)();
}

float Project::Chunk::AskBlockProperty(const int x, const int y, const int z, float(Block::* prop)() const) const {
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
            float noise0 = glm::simplex(glm::vec2(voxel_x / 40.f, voxel_z / 40.f));
            float noise1 = glm::perlin(glm::vec2(voxel_x / 36.f, voxel_z / 36.f));
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
    if (x == 0) {
        this->negative_x->SuggestReMesh();
    } else if (x == 15) {
        this->positive_x->SuggestReMesh();
    }
    if (z == 0) {
        this->negative_z->SuggestReMesh();
    } else if (z == 15) {
        this->positive_z->SuggestReMesh();
    }
}

bool Project::Chunk::AskBlockProperty(const int x, const int y, const int z, bool(Block::* const prop)() const) const {
    std::shared_lock lock{this->mutex};
    return (*this->operator()(x, y, z).*prop)();
}

Project::Block*& Project::Chunk::operator()(const int x, const int y, const int z) {
    if (x < 0 && z < 0) {
        return this->diag_neg_neg_xz->operator()(x + CHUNK_SIZE, y, z + CHUNK_SIZE);
    } else if (x >= CHUNK_SIZE && z >= CHUNK_SIZE) {
        return this->diag_pos_pos_xz->operator()(x - CHUNK_SIZE, y, z - CHUNK_SIZE);
    } else if (x < 0 && z >= CHUNK_SIZE) {
        return this->diag_neg_pos_xz->operator()(x + CHUNK_SIZE, y, z - CHUNK_SIZE);
    } else if (x >= CHUNK_SIZE && z < 0) {
        return this->diag_pos_neg_xz->operator()(x - CHUNK_SIZE, y, z + CHUNK_SIZE);
    } else if (x < 0) {
        return this->negative_x->operator()(x + CHUNK_SIZE, y, z);
    } else if (z < 0) {
        return this->negative_z->operator()(x, y, z + CHUNK_SIZE);
    } else if (x >= CHUNK_SIZE) {
        return this->positive_x->operator()(x - CHUNK_SIZE, y, z);
    } else if (z >= CHUNK_SIZE) {
        return this->positive_z->operator()(x, y, z - CHUNK_SIZE);
    }
    return data.at(x * CHUNK_SIZE * CHUNK_DEPTH + y * CHUNK_SIZE + z);
}

const Project::Block* Project::Chunk::operator()(const int x, const int y, const int z) const {
    if (x < 0 && z < 0) {
        const Chunk* pointer = this->diag_neg_neg_xz;
        return pointer->operator()(x + CHUNK_SIZE, y, z + CHUNK_SIZE);
    } else if (x >= CHUNK_SIZE && z >= CHUNK_SIZE) {
        const Chunk* pointer = this->diag_pos_pos_xz;
        return pointer->operator()(x - CHUNK_SIZE, y, z - CHUNK_SIZE);
    } else if (x < 0 && z >= CHUNK_SIZE) {
        const Chunk* pointer = this->diag_neg_pos_xz;
        return pointer->operator()(x + CHUNK_SIZE, y, z - CHUNK_SIZE);
    } else if (x >= CHUNK_SIZE && z < 0) {
        const Chunk* pointer = this->diag_pos_neg_xz;
        return pointer->operator()(x - CHUNK_SIZE, y, z + CHUNK_SIZE);
    } else if (x < 0) {
        const Chunk* pointer = this->negative_x;
        return pointer->operator()(x + CHUNK_SIZE, y, z);
    } else if (z < 0) {
        const Chunk* pointer = this->negative_z;
        return pointer->operator()(x, y, z + CHUNK_SIZE);
    } else if (x >= CHUNK_SIZE) {
        const Chunk* pointer = this->positive_x;
        return pointer->operator()(x - CHUNK_SIZE, y, z);
    } else if (z >= CHUNK_SIZE) {
        const Chunk* pointer = this->positive_z;
        return pointer->operator()(x, y, z - CHUNK_SIZE);
    }
    return data.at(x * CHUNK_SIZE * CHUNK_DEPTH + y * CHUNK_SIZE + z);
}

Project::Block*& Project::Chunk::operator()(const glm::ivec3& v) {
    int x = v.x, y = v.y, z = v.z;
    return this->operator()(x, y, z);
}

const Project::Block* Project::Chunk::operator()(const glm::ivec3& v) const {
    int x = v.x, y = v.y, z = v.z;
    return this->operator()(x, y, z);
}

void Project::Chunk::SetNeighbours(Chunk* top, Chunk* right, Chunk* bottom, Chunk* left, Chunk* top_right, Chunk* bot_right, Chunk* bot_left, Chunk* top_left) {
    // the chunks neighbours from a bird's eye view (looking down into negative y axis)
    this->negative_z = top;
    this->positive_z = bottom;
    this->positive_x = right;
    this->negative_x = left;
    this->diag_pos_neg_xz = top_right;
    this->diag_pos_pos_xz = bot_right;
    this->diag_neg_pos_xz = bot_left;
    this->diag_neg_neg_xz = top_left;
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
    static constexpr glm::ivec3 vertex_ao_lookup[] = {
        { -1, 1, 1 },
        { 1, 1, 1 },
        { -1, -1, 1 },
        { 1, -1, 1 },
        { -1, 1, -1 },
        { 1, 1, -1 },
        { -1, -1, -1 },
        { 1, -1, -1 }
    };
    static constexpr glm::ivec3 face_ao_offset_iter[6][8][3] = {
        {
            { { -1, 1, 1 }, { -1, 0, 1 }, { 0, 1, 1 } }, // vertex 0 face 0
            { { 1, 1, 1 }, { 1, 0, 1 }, { 0, 1, 1 } }, // vertex 1 face 0
            { { -1, -1, 1 }, { -1, 0, 1 }, { 0, -1, 1 } }, // vertex 2 face 0
            { { 1, -1, 1 }, { 1, 0, 1 }, { 0, -1, 1 } }, // vertex 3 face 0
        },
        {
            { { -1, 1, 1 }, { -1, 1, 0 }, { 0, 1, 1 } }, // vertex 0 face 1
            { { 1, 1, 1 }, { 1, 1, 0 }, { 0, 1, 1 } }, // vertex 1 face 1
            {},
            {},
            { { -1, 1, -1 }, { -1, 1, 0 }, { 0, 1, -1 } }, // vertex 4 face 1
            { { 1, 1, -1 }, { 1, 1, 0 }, { 0, 1, -1 } }, // vertex 5 face 1
        },
        {
            {},
            { { 1, 1, 1 }, { 1, 0, 1 }, { 1, 1, 0 } }, // vertex 1 face 2
            {},
            { { 1, -1, 1 }, { 1, 0, 1 }, { 1, -1, 0 } }, // vertex 3 face 2
            {},
            { { 1, 1, -1 }, { 1, 0, -1 }, { 1, 1, 0 } }, // vertex 5 face 2
            {},
            { { 1, -1, -1 }, { 1, 0, -1 }, { 1, -1, 0 } }, // vertex 7 face 2
        },
        {
            {},
            {},
            { { -1, -1, 1 }, { -1, -1, 0 }, { 0, -1, 1 } }, // vertex 2 face 3
            { { 1, -1, -1 }, { 1, -1, 0 }, { 0, -1, 1 } }, // vertex 3 face 3
            {},
            {},
            { { -1, -1, -1 }, { -1, -1, 0 }, { 0, -1, -1 } }, // vertex 6 face 3
            { { 1, -1, -1 }, { 1, -1, 0 }, { 0, -1, -1 } }, // vertex 7 face 3
        },
        {
            { { -1, 1, 1 }, { -1, 0, 1 }, { -1, 1, 0 } }, // vertex 0 face 4
            {},
            { { -1, -1, 1 }, { -1, 0, 1 }, { -1, -1, 0 } }, // vertex 2 face 4
            {},
            { { -1, 1, -1 }, { -1, 0, -1 }, { -1, 1, 0 } }, // vertex 4 face 4
            {},
            { { -1, -1, -1 }, { -1, 0, -1 }, { -1, -1, 0 } }, // vertex 6 face 4
        },
        {
            {},
            {},
            {},
            {},
            { { -1, 1, -1 }, { -1, 0, -1 }, { 0, 1, -1 } }, // vertex 4 face 5
            { { 1, 1, -1 }, { 1, 0, -1 }, { 0, 1, -1 } }, // vertex 5 face 5
            { { -1, -1, -1 }, { -1, 0, -1 }, { 0, -1, -1 } }, // vertex 6 face 5
            { { 1, -1, -1 }, { 1, 0, -1 }, { 0, -1, -1 } }, // vertex 7 face 5
        },
    };
    this->counter = 0;
    for (int r{0}; r < Chunk::CHUNK_SIZE; r++) {
        for (int c{0}; c < Chunk::CHUNK_SIZE; c++) {
            for (int y{0}; y < Chunk::CHUNK_DEPTH; y++) {
                if (this->operator()(r, y, c)->SkipRender()) {
                    continue;
                }
                glm::ivec3 start(r, y, c);
                unsigned int texture_index_start = 3U * static_cast<unsigned int>(this->operator()(r, y, c)->GetID());
                unsigned int pos = (r << 12) + (y) + (c << 8);
                auto FaceLambda = [&](const int row, const int col, const int depth, unsigned int face){
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
                                this->mesh.push_back(0);
                            }
                            glm::ivec3 corner_vec = start + face_ao_offset_iter[face][vertex_index][0];
                            glm::ivec3 side1_vec = start + face_ao_offset_iter[face][vertex_index][1];
                            glm::ivec3 side2_vec = start + face_ao_offset_iter[face][vertex_index][2];
                            bool corner = corner_vec.y < 0 || corner_vec.y >= CHUNK_DEPTH || this->operator()(corner_vec)->IsOpaque();
                            bool side1 = side1_vec.y < 0 || side1_vec.y >= CHUNK_DEPTH || this->operator()(side1_vec)->IsOpaque();
                            bool side2 = side2_vec.y < 0 || side2_vec.y >= CHUNK_DEPTH || this->operator()(side2_vec)->IsOpaque();
                            unsigned int vertex = pos | (vertex_index << 16) | (texture_index << 20) | (uv_index << 28);
                            unsigned int vertex_long = 3U - ((side1 && side2) ? 0 : (3U - side1 - corner - side2));
                            this->mesh[this->counter] = vertex_long;
                            this->counter++;
                            this->mesh[this->counter] = vertex;
                            this->counter++;
                        }
                    }
                };
                FaceLambda(r + 1, c, y, 2);
                FaceLambda(r - 1, c, y, 4);
                FaceLambda(r, c, y + 1, 1);
                FaceLambda(r, c, y - 1, 3);
                FaceLambda(r, c - 1, y, 5);
                FaceLambda(r, c + 1, y, 0);
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
        glVertexAttribIPointer(1, 2, GL_UNSIGNED_INT, 0, (void*)0);
        glEnableVertexAttribArray(1);
        this->gl_inited = true;
    }
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
    if (this->counter > this->last_counter) {
        glBufferData(GL_ARRAY_BUFFER, (int)this->counter * sizeof(unsigned int), &this->mesh.at(0), GL_DYNAMIC_DRAW);
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
    // important: need to draw wtih half the counter or else we will be doing way too many draws
    glDrawArrays(GL_TRIANGLES, 0, this->render_counter / 2);
}

void Project::Chunk::SuggestReMesh() {
    this->needs_remeshing = true;
}

bool Project::Chunk::NeedsRemeshing() const {
    return this->needs_remeshing;
}

void Project::Chunk::ResetNeedsMeshing() {
    this->needs_remeshing = false;
}

bool Project::Chunk::IsFinishedGenerating() {
    return this->chunk_ready;
}

bool Project::Chunk::IsMeshReady() const {
    return this->mesh_ready;
}