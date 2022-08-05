#pragma once

#include "hud_element.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <display/display.hpp>

namespace Project {
    class Program;
    
    class Crosshair : public HudElement {
    private:
        constexpr static float WIDTH = 8.f;
        glm::mat4 scale_trans_matrix = glm::scale(glm::translate(glm::mat4(1.f), glm::vec3(Display::GetInstance().GetWidth() / 2 - WIDTH / 2, Display::GetInstance().GetHeight() / 2 - WIDTH / 2, 0.f)), glm::vec3(WIDTH, WIDTH, 1.f));
        int current_width = Display::GetInstance().GetWidth();
        int current_height = Display::GetInstance().GetWidth();
    public:
        virtual void Render(Program* shader) override;
    };
}