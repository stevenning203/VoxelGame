#include "crosshair.hpp"
#include <glad/glad.h>
#include <shader/program.hpp>
#include <generic/glm_helper.hpp>
#include <display/display.hpp>

void Project::Crosshair::Render(Program* shader) {
    static constexpr float vertices[] = {
        0.f, 0.f,
        0.f, 1.f,
        1.f, 1.f,
        1.f, 1.f,
        1.f, 0.f,
        0.f, 0.f,
    };
    glm::mat4 scale_trans_matrix = glm::scale(glm::translate(glm::mat4(1.f), glm::vec3(Display::GetInstance().GetWidth() / 2 - WIDTH / 2, Display::GetInstance().GetHeight() / 2 - WIDTH / 2, 0.f)), glm::vec3(WIDTH, WIDTH, 1.f));
    if (first) {
        glGenVertexArrays(1, &this->vao_id);
        glBindVertexArray(this->vao_id);
        glGenBuffers(1, &this->vbo_id);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(2);
        first = false;
    }
    shader->UniformMatrix("pixel_scale_matrix", scale_trans_matrix);
    shader->UniformInt("render_2d", 1);
    glBindVertexArray(this->vao_id);
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
    shader->UniformInt("render_2d", 0);
}