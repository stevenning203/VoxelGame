#include "game.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <display/display.hpp>
#include <shader/shader.hpp>
#include <shader/program.hpp>
#include <input/mouse_handler.hpp>
#include <camera/camera.hpp>
#include <chunk/chunk_manager.hpp>
#include <world_renderer/world_renderer.hpp>
#include <input/key_handler.hpp>
#include <texture_atlas/texture_atlas.hpp>
#include <thread>
#include <physics/world_collision_handler.hpp>
#include <generic/workable.hpp>
#include <player/player.hpp>
#include <generic/debug.hpp>
#include <thread/worker_thread.hpp>

void Project::Game::GameLogicLoop() {
    while (!Display::GetInstance().ShouldClose()) {
        for (Workable* worker : this->modules) {
            worker->ThreadWork();
        }
    }
}

void Project::Game::ExpensiveWorkLoop() {
    while (!Display::GetInstance().ShouldClose()) {
        for (Workable* worker : this->modules) {
            worker->ExpensiveThreadWork();
        }
    }
}

void Project::Game::RenderLoop() {
    while (!Display::GetInstance().ShouldClose()) {
        this->timer->Sleep(std::max(0, 1000000000 / Display::INITIAL_FPS - this->timer->GetDeltaHighResolution()));
        this->timer->HighResolutionMeasureFirst();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();

        for (Workable* worker : this->modules) {
            worker->MainThreadWork();
        }

        Display::GetInstance().SwapBuffers();
        this->timer->HighResolutionMeasureSecond();
    }
}

void Project::Game::Main() {
    std::thread logic_thread_loop([this](){ this->GameLogicLoop(); });
    std::thread expensive_loop([this](){ this->ExpensiveWorkLoop(); });
    RenderLoop();
    logic_thread_loop.join();
}

Project::Game::Game() {
    Display::GetInstance().Init(1280, 720, "Project F");
    Shader vertex_shader("res/shaders/vertex.glsl", false);
    Shader fragment_shader("res/shaders/fragment.glsl", true);
    Program* shader = new Program(vertex_shader, fragment_shader);
    shader->Use();
    MouseHandler* mouse = new MouseHandler();
    KeyHandler* keyboard = new KeyHandler();
    Camera* camera = new Camera(shader, mouse, keyboard, nullptr);
    Player* player = new Player(camera);
    
    (new TextureAtlas("assets/atlas.png"))->Bind();
    Display::GetInstance().SetShader(shader);
    Display::GetInstance().SuggestDimensions();
    Display::GetInstance().Subscribe(*mouse);
    Display::GetInstance().Subscribe(*keyboard);

    ChunkManager* world = new ChunkManager(player, mouse, shader);
    WorldRenderer* renderer = new WorldRenderer(world, shader);
    WorldCollisionHandler* collision_handler = new WorldCollisionHandler(world, camera, mouse, player);

    keyboard->Subscribe(*player);
    mouse->Subscribe(*world);

    WorkerThread main_thread;
    main_thread.AddWorkable()

    this->modules.push_back(camera);
    this->modules.push_back(renderer);
    this->modules.push_back(world);
    this->modules.push_back(collision_handler);
    //this->modules.push_back(timer);
    this->modules.push_back(mouse);
    this->modules.push_back(keyboard);
    
    this->Main();
    glfwTerminate();
}