#include "world_renderer.hpp"
#include <chunk/chunk.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/program.hpp>
#include <texture_atlas/texture_atlas.hpp>
#include <chunk/chunk_manager.hpp>
#include <hud/hud_element.hpp>
#include <hud/crosshair.hpp>

void Project::WorldRenderer::RenderChunkManager() {
    static auto func = [](std::pair<const std::pair<int, int>, Chunk*>& pair, Program* shader) {
        int x = pair.first.first;
        int z = pair.first.second;
        shader->UniformFloat("chunk_offset_x", static_cast<float>(x * Chunk::CHUNK_SIZE));
        shader->UniformFloat("chunk_offset_z", static_cast<float>(z * Chunk::CHUNK_SIZE));
        pair.second->Render();
    };
    this->chunk_manager->ForEachMut(func, this->shader);
}

Project::WorldRenderer::WorldRenderer(ChunkManager* cm, Program* shader) : shader(shader), chunk_manager(cm) {
    this->block_atlas = new TextureAtlas("assets/atlas.png");
    this->hud_elements.push_back(new Crosshair());
}

void Project::WorldRenderer::MainThreadWork() {
    this->RenderChunkManager();

    // important : this should be done last to prevent depth issues
    this->RenderHUD();
}

void Project::WorldRenderer::ThreadWork() {

}

void Project::WorldRenderer::RenderHUD() {
    for (HudElement*& hud_element : this->hud_elements) {
        hud_element->Render(this->shader);
    }
}