#pragma once

#include <vector>

namespace Project {
    class Workable;
    class WorkerThread;

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
         * This thread does work based on the PFPS defined in this module. It pushes physics frames when they are "overdue".
         * 
         */
        void GameLogicLoop();

        /**
         * @brief offload expensive work to a thread where timing is not important
         * 
         * This thread does work ASAP, so any work that is pushed to it during the loop will be immediately called with NO restraints.
         * 
         */
        void ExpensiveWorkLoop();

        /**
         * @brief the rendering loop
         * 
         * This thread does work based on the FPS defined in Display::. Should a frame take any less than that time, the thread will wait until the required time has past
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