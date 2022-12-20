#pragma once
#include <vector>
#include <generic/workable.hpp>
#include <generic/input_observer.hpp>

namespace Project {
    class HudElement;

    /**
     * @brief the hud that holds all the hud elements and renders them
     * 
     */
    class Hud : public Workable, public InputObserver {
        std::vector<HudElement*> elements;

        /**
         * @brief render all the hud elements
         * 
         */
        void Render();
    public:
        virtual void ThreadWork() override;

        virtual void MainThreadWork() override;
    };
}