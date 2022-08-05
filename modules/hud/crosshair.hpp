#pragma once

#include "hud_element.hpp"
#include <glm/glm.hpp>

namespace Project {
    class Program;
    
    class Crosshair : public HudElement {
    private:
        constexpr static float WIDTH = 8.f;
    public:
        virtual void Render(Program* shader) override;
    };
}