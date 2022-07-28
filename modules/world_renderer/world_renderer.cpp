#include "world_renderer.hpp"
#include <chunk/chunk.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/program.hpp>
#include <texture_atlas/texture_atlas.hpp>
#include <chunk/chunk_manager.hpp>

void Project::WorldRenderer::RenderChunkManager() {
    this->chunk_manager->ForEachMut(std::bind([](std::pair<const std::pair<int, int>, Chunk*>& pair, Program* shader){
        if (!pair.second->IsMeshReady()) {
            return;
        }
        int x = pair.first.first;
        int z = pair.first.second;
        shader->UniformFloat("chunk_offset_x", static_cast<float>(x * Chunk::CHUNK_SIZE));
        shader->UniformFloat("chunk_offset_z", static_cast<float>(z * Chunk::CHUNK_SIZE));
        pair.second->Render();
    }, std::placeholders::_1, this->shader));
}

Project::WorldRenderer::WorldRenderer(ChunkManager* cm, Program* shader) : shader(shader), chunk_manager(cm) {
    this->block_atlas = new TextureAtlas("assets/atlas.png");
}

void Project::WorldRenderer::MainThreadWork() {
    this->RenderChunkManager();
}

void Project::WorldRenderer::ThreadWork() {

}