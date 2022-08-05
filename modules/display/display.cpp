#include <shader/program.hpp>
#include "display.hpp"
#include <stb/stb_image.h>

Project::Display::Display() : should_close(false) {

}

Project::Display& Project::Display::GetInstance() {
    static Display display;
    return display;
}

void Project::Display::Init(const int width, const int height, const char* title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    this->pointer = glfwCreateWindow(width, height, title, NULL, NULL);
    if (this->pointer == nullptr) {
        throw std::runtime_error("Cannot initialize window");
    }
    glfwMakeContextCurrent(this->pointer);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Cannot initialize GLAD");
    }
    glfwSetFramebufferSizeCallback(this->pointer, _FRAMEBUFFER_CALLBACK);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSetInputMode(Display::GetInstance().GetWindowPointer(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(0);
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    stbi_set_flip_vertically_on_load(1);
}

void Project::Display::SwapBuffers() {
    glfwSwapBuffers(this->pointer);
}

void Project::Display::SetShader(Program* shader) {
    this->shader = shader;
    SuggestDimensions();
}

void Project::Display::PushMatrix() {
    if (shader == nullptr) {
        throw std::runtime_error("Shader has not been set in display");
    }
    shader->UniformMatrix("projection_matrix", this->projection_matrix);
}

void Project::Display::PushOrtho() {
    if (shader == nullptr) {
        throw std::runtime_error("shader has not been set in display");
    }
    shader->UniformMatrix("orthographic_matrix", this->orthographic_matrix);
}

bool Project::Display::ShouldClose() {
    return glfwWindowShouldClose(this->pointer) || should_close;
}

void Project::Display::HintClose() {
    this->should_close = true;
}

GLFWwindow* Project::Display::GetWindowPointer() {
    return this->pointer;
}

void Project::Display::SuggestDimensions() {
    int w, h;
    glfwGetFramebufferSize(this->pointer, &w, &h);
    glViewport(0, 0, w, h);
    this->projection_matrix = glm::perspective(INITIAL_FOV, (float)w / (float)h, 0.1f, 500.f);
    this->orthographic_matrix = glm::ortho(0.f, (float)w, (float)h, 0.f, -1.f, 1.f);
    this->width = w;
    this->height = h;
    PushMatrix();
    PushOrtho();
}

void Project::_FRAMEBUFFER_CALLBACK(GLFWwindow* w, int width, int height) {
    Display::GetInstance().SuggestDimensions();
}

int Project::Display::GetHeight() {
    return this->height;
}

int Project::Display::GetWidth() {
    return this->width;
}