#include "world_renderer.hpp"
#include <chunk/chunk_manager.hpp>
#include <chunk/chunk.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/program.hpp>
#include <texture_atlas/texture_atlas.hpp>

void Project::WorldRenderer::RenderChunkManager(ChunkManager& cm, Program& p, TextureAtlas& atlas) {
    for (auto& pair : cm) {
        int x = pair.second->GetRow();
        int z = pair.second->GetCol();
        pair.second->PushMesh();
        p.UniformFloat("chunk_offset_x", static_cast<float>(x * Chunk::CHUNK_SIZE));
        p.UniformFloat("chunk_offset_z", static_cast<float>(z * Chunk::CHUNK_SIZE));
        glDrawArrays(GL_TRIANGLES, 0, pair.second->GetCounter());
    }
}