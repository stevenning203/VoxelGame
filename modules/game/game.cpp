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
#include <chunk/chunk_mesh_manager.hpp>
#include <world_renderer/world_renderer.hpp>
#include <input/key_handler.hpp>
#include <texture_atlas/texture_atlas.hpp>
#include <thread>
#include <physics/world_collision_handler.hpp>

extern Project::Display display;

void Project::Game::GameLogicLoop() {
    while (!display.ShouldClose()) {
        this->world->UpdatePlayerVisibleChunks(this->camera->GetPosition());
        this->world->Work();
        //this->collision_handler->EnablePlayerBlockDestruction(*this->world, *this->camera, *this->mouse, WorldCollisionHandler::PLAYER_REACH);
    }
}

void Project::Game::RenderLoop() {
    while (!display.ShouldClose()) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->mesher->GenerateQueuedMeshes();
        this->mesher->ReMeshFlaggedMeshes();
        this->renderer->RenderChunkMeshManager(*this->mesher, *this->shader, *this->atlas);
        
        display.SwapBuffers();
        this->camera->PushMatrix(*shader);
        this->camera->UpdatePanning(*mouse, *timer);
        this->camera->UpdateMovement(*keyboard, *timer);
        
        this->timer->Update();
        if (this->mouse->GetMouseState(Project::MouseHandler::MouseEnum::RMB_DOWN)) {
            display.HintClose();
        }
        this->mouse->Update(display);
        this->keyboard->Update();
        this->timer->Sleep(5);
    }
}

void Project::Game::Main() {
    std::thread logic_thread_loop([this](){this->GameLogicLoop(); });
    RenderLoop();
    logic_thread_loop.join();
}

Project::Game::Game() {
    display.Init(1280, 720, "Project F");
    Shader vertex_shader("res/shaders/vertex.glsl", false);
    Shader fragment_shader("res/shaders/fragment.glsl", true);
    this->shader = new Program(vertex_shader, fragment_shader);
    shader->Use();
    this->mouse = new MouseHandler(display);
    this->keyboard = new KeyHandler(display);
    this->timer = new Timer();
    this->camera = new Camera();
    this->mesher = new ChunkMeshManager(this->world);
    
    this->atlas = new TextureAtlas("assets/atlas.png");
    this->atlas->Bind();
    display.SetShader(shader);
    display.SuggestDimensions();

    this->world = new ChunkManager(this->mesher);
    this->renderer = new WorldRenderer();
    this->collision_handler = new WorldCollisionHandler();

    this->Main();
    glfwTerminate();
}