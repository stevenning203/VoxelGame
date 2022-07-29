#include "game.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <display/display.hpp>
#include <shader/shader.hpp>
#include <shader/program.hpp>
#include <input/mouse_handler.hpp>
#include <input/timer.hpp>
#include <camera/camera.hpp>
#include <chunk/chunk_manager.hpp>
#include <world_renderer/world_renderer.hpp>
#include <input/key_handler.hpp>
#include <texture_atlas/texture_atlas.hpp>
#include <thread>
#include <physics/world_collision_handler.hpp>
#include <generic/workable.hpp>
#include <player/player.hpp>

unsigned int vao;
bool flag = true;

void Project::Game::GameLogicLoop() {
    while (!Display::GetInstance().ShouldClose()) {
        for (Workable* worker : this->modules) {
            worker->ThreadWork();
        }
    }
}

void Project::Game::RenderLoop() {
    while (!Display::GetInstance().ShouldClose()) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (Workable* worker : this->modules) {
            worker->MainThreadWork();
        }

        Display::GetInstance().SwapBuffers();
    }
}

void Project::Game::Main() {
    std::thread logic_thread_loop([this](){ this->GameLogicLoop(); });
    RenderLoop();
    logic_thread_loop.join();
}

Project::Game::Game() {
    Display::GetInstance().Init(1280, 720, "Project F");
    Shader vertex_shader("res/shaders/vertex.glsl", false);
    Shader fragment_shader("res/shaders/fragment.glsl", true);
    Program* shader = new Program(vertex_shader, fragment_shader);
    Player* player = new Player();
    shader->Use();
    MouseHandler* mouse = new MouseHandler();
    KeyHandler* keyboard = new KeyHandler();
    Timer* timer = new Timer();
    Camera* camera = new Camera(shader, mouse, keyboard, timer);
    
    TextureAtlas* atlas = new TextureAtlas("assets/atlas.png");
    atlas->Bind();
    Display::GetInstance().SetShader(shader);
    Display::GetInstance().SuggestDimensions();

    ChunkManager* world = new ChunkManager(player);
    WorldRenderer* renderer = new WorldRenderer(world, shader);
    WorldCollisionHandler* collision_handler = new WorldCollisionHandler(world, camera, mouse, player);

    this->modules.push_back(world);
    this->modules.push_back(renderer);
    this->modules.push_back(collision_handler);
    this->modules.push_back(camera);
    this->modules.push_back(timer);
    this->modules.push_back(mouse);
    this->modules.push_back(keyboard);
    
    this->Main();
    glfwTerminate();
}