#pragma once

#include "hud_element.hpp"

namespace Project {
    class Program;
    
    class Crosshair : public HudElement {
    private:
        constexpr static float DIMENSIONS = 20.f;
    public:
        virtual void Render(Program* shader) override;
    };
}