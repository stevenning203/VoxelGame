#pragma once

#include <vector>

namespace Project {
    class Workable;

    /**
     * @brief The game class that holds all the game logic.
     * 
     */
    class Game {
        std::vector<Workable*> modules;

        /**
         * @brief the main loop
         * 
         */
        void Main();

        /**
         * @brief the game logic loop
         * 
         */
        void GameLogicLoop();

        /**
         * @brief the rendering loop
         * 
         */
        void RenderLoop();

    public:
        /**
         * @brief Initialize all OpenGL requirements for the game, and initialize all game aspects.
         * 
         */
        Game();
    };
}