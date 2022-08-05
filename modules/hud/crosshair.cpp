#include "crosshair.hpp"
#include <glad/glad.h>
#include <shader/program.hpp>
#include <generic/glm_helper.hpp>

void Project::Crosshair::Render(Program* shader) {
    static constexpr float vertices[] = {
        -1.f, DIMENSIONS,
        -1.f, -DIMENSIONS,
        1.f, -DIMENSIONS,
        1.f, -DIMENSIONS,
        1.f, DIMENSIONS,
        -1.f, DIMENSIONS,
         DIMENSIONS,-1.f,
         -DIMENSIONS,-1.f,
        -DIMENSIONS,1.f, 
        -DIMENSIONS,1.f, 
        DIMENSIONS,1.f, 
         DIMENSIONS  -1.f,
    };
    if (first) {
        glGenVertexArrays(1, &this->vao_id);
        glBindVertexArray(this->vao_id);
        glGenBuffers(1, &this->vbo_id);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
        glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(2);
        first = false;
    }
    shader->UniformMatrix("pixel_scale_matrix", ntd::IdentityMat4());
    shader->UniformInt("render_2d", 1);
    glBindVertexArray(this->vao_id);
    glDrawArrays(GL_TRIANGLES, 0, 12);
}