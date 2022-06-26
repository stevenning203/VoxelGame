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
    camera.GetPosition() = glm::vec3(5.f);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    Project::ChunkManager world;
    world.WorldGen();
    Project::WorldRenderer renderer;

    float vertices[] = {
        -0.5f, 0.5f, 0.5f, 
        0.5f, 0.5f, 0.5f, 
        -0.5f, -0.5f, 0.5f, 
        0.5f, -0.5f, 0.5f, 
        -0.5f, 0.5f, -0.5f, 
        0.5f, 0.5f,- 0.5f, 
        -0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f, 
    };

    unsigned int indices[] = {
        0, 2, 1,
        2, 3, 1, // front face
        1, 3, 7,
        1, 7, 5, // right face
        0, 6, 2,
        0, 4, 6, // left face
        2, 7, 3,
        2, 6, 7, // bottom face
        0, 5, 4,
        0, 1, 5, // top face
        4, 5, 6, 
        6, 5, 7, // back face
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    glfwSetInputMode(display.GetWindowPointer(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // the main loop
    glfwSwapInterval(0);
    while (!display.ShouldClose() and !mouse.GetMouseState(Project::MouseHandler::MouseEnum::RMB_DOWN)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //begin logic

        //end logic

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        renderer.RenderChunkManager(world, shader);

        display.SwapBuffers();
        camera.PushMatrix(shader);
        //shader.PushMVPMatrix(camera, display, identity);
        camera.UpdatePanning(mouse, timer);
        camera.UpdateMovement(keyboard, timer);
        
        timer.Update();
        mouse.Update(display);
        keyboard.Update();
    }
    glfwTerminate();
    return 0;
}