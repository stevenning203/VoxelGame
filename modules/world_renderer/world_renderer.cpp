#include "world_renderer.hpp"
#include <chunk/chunk_mesh_manager.hpp>
#include <chunk/chunk.hpp>
#include <chunk/chunk_mesh.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/program.hpp>
#include <texture_atlas/texture_atlas.hpp>

void Project::WorldRenderer::RenderChunkMeshManager(ChunkMeshManager& cm, Program& p, TextureAtlas& atlas) {
    cm.ForEachMut([](std::pair<const std::pair<int, int>, ChunkMesh*>& pair, Program& p){
        if (!pair.second->GetReady()) {
            return;
        }
        int x = pair.first.first;
        int z = pair.first.second;
        glBindVertexArray(pair.second->GetVAOID());
        p.UniformFloat("chunk_offset_x", static_cast<float>(x * Chunk::CHUNK_SIZE));
        p.UniformFloat("chunk_offset_z", static_cast<float>(z * Chunk::CHUNK_SIZE));
        glDrawArrays(GL_TRIANGLES, 0, pair.second->GetCounter());
    }, p);
}