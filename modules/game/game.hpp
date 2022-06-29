#pragma once

namespace Project {
    class Timer;
    class KeyHandler;
    class MouseHandler;
    class Display;
    class Player;
    class Camera;
    class Program;
    class ChunkManager;
    class WorldRenderer;

    /**
     * @brief The game class that holds all the game logic.
     * 
     */
    class Game {
        Timer* timer;
        KeyHandler* keyboard;
        MouseHandler* mouse;
        Player* player;
        Camera* camera;
        Program* shader;
        ChunkManager* world;
        WorldRenderer* renderer;
        Player* player;

        /**
         * @brief the main loop
         * 
         */
        void Main();

    public:
        /**
         * @brief Initialize all OpenGL requirements for the game, and initialize all game aspects.
         * 
         */
        Game();
    };
}