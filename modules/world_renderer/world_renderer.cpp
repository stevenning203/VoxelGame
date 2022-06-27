#include "world_renderer.hpp"
#include <chunk/chunk_manager.hpp>
#include <chunk/chunk.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/program.hpp>

void Project::WorldRenderer::RenderChunkManager(ChunkManager& cm, Program& p) {
    std::vector<Project::Chunk>& v = cm.GetChunks();
    for (Chunk& chunk : v) {
        glm::mat4 model_matrix = glm::translate(glm::mat4(1.f),
            glm::vec3(chunk.GetRow() * Chunk::CHUNK_SIZE, 0, chunk.GetCol() * Chunk::CHUNK_SIZE));
        model_matrix = glm::mat4(1.f);
        chunk.PushMesh();
        p.UniformMatrix("chunk_matrix", model_matrix);
        glDrawArrays(GL_TRIANGLES, 0, chunk.GetCounter());
    }
}