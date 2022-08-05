#pragma once

namespace Project {
    class Program;

    /**
     * @brief element of a hud
     * 
     */
    class HudElement {
    protected:
        bool first = true;
        unsigned int vao_id, vbo_id;
    public:
        /**
         * @brief draw the hud element on the screen
         * 
         * @param shader the shader
         */
        virtual void Render(Program* shader) = 0;

        virtual ~HudElement() = default;
    };
}