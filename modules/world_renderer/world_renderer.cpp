#include "world_renderer.hpp"
#include <chunk/chunk_manager.hpp>
#include <chunk/chunk.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/program.hpp>

void Project::WorldRenderer::RenderChunkManager(ChunkManager& cm, Program& p) {
    std::vector<Project::Chunk>& v = cm.GetChunks();
    for (Chunk& chunk : v) {
        for (int r{0}; r < Chunk::CHUNK_SIZE; r++) {
            for (int c{0}; c < Chunk::CHUNK_SIZE; c++) {
                for (int y{0}; y < Chunk::CHUNK_DEPTH; y++) {
                    glm::mat4 model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(r, y, c));
                    p.UniformMatrix("chunk_matrix", model_matrix);
                    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
                }
            }
        }
    }
}