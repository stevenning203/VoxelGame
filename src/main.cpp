#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <display/display.hpp>
#include <shader/program.hpp>
#include <shader/shader.hpp>
#include <input/mouse_handler.hpp>
#include <input/timer.hpp>
#include <camera/camera.hpp>
#include <chunk/chunk_manager.hpp>
#include <chunk/chunk.hpp>
#include <world_renderer/world_renderer.hpp>
#include <input/key_handler.hpp>

extern Project::Display display;

int main() {
    display.Init(1280, 720, "Project F");
    Project::Shader vertex_shader("res/shaders/vertex.glsl", false);
    Project::Shader fragment_shader("res/shaders/fragment.glsl", true);
    Project::Program shader(vertex_shader, fragment_shader);
    shader.Use();
    Project::MouseHandler mouse(display);
    Project::KeyHandler keyboard(display);
    Project::Timer timer;
    Project::Camera camera;
    display.SetShader(&shader);
    display.SuggestDimensions();
    glClearColor(0.8f, 0.9f, 0.8f, 1.0f);

    Project::ChunkManager world;
    world.WorldGen();
    Project::WorldRenderer renderer;

    glfwSetInputMode(display.GetWindowPointer(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(0);

    world.SuggestRemesh();

    while (!display.ShouldClose() and !mouse.GetMouseState(Project::MouseHandler::MouseEnum::RMB_DOWN)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //begin logic
        
        //end logic

        renderer.RenderChunkManager(world, shader);
        //glBindVertexArray(vao);
        //glDrawArrays(GL_TRIANGLES, 0, 6);

        display.SwapBuffers();
        camera.PushMatrix(shader);
        //shader.PushMVPMatrix(camera, display, identity);
        camera.UpdatePanning(mouse, timer);
        camera.UpdateMovement(keyboard, timer);
        
        timer.Update();
        mouse.Update(display);
        keyboard.Update();
        timer.Sleep(5);
    }
    glfwTerminate();
    return 0;
}