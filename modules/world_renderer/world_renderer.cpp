#include "world_renderer.hpp"
#include <chunk/chunk_manager.hpp>
#include <chunk/chunk.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/program.hpp>

void Project::WorldRenderer::RenderChunkManager(ChunkManager& cm, Program& p) {
    std::vector<Project::Chunk>& v = cm.GetChunks();
    for (Chunk& chunk : v) {
        int x = chunk.GetRow();
        int z = chunk.GetCol();
        chunk.PushMesh();
        p.UniformFloat("chunk_offset_x", static_cast<float>(x * Chunk::CHUNK_SIZE));
        p.UniformFloat("chunk_offset_z", static_cast<float>(z * Chunk::CHUNK_SIZE));
        glDrawArrays(GL_TRIANGLES, 0, chunk.GetCounter());
    }
}