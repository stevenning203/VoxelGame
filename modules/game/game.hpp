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
    class TextureAtlas;
    class ChunkMeshManager;
    class DDACaster;
    class WorldCollisionHandler;

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
        TextureAtlas* atlas;
        ChunkMeshManager* mesher;
        WorldCollisionHandler* collision_handler;

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