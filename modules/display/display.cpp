#include <shader/program.hpp>
#include "display.hpp"

Project::Display display;

Project::Display::Display() {

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
    if (this->pointer== nullptr) {
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
    glfwSetInputMode(display.GetWindowPointer(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(0);
    glClearColor(0.8f, 0.9f, 0.8f, 1.0f);
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

bool Project::Display::ShouldClose() {
    return glfwWindowShouldClose(this->pointer);
}

GLFWwindow* Project::Display::GetWindowPointer() {
    return this->pointer;
}

void Project::Display::SuggestDimensions() {
    int w, h;
    glfwGetFramebufferSize(this->pointer, &w, &h);
    glViewport(0, 0, w, h);
    this->projection_matrix = glm::perspective(INITIAL_FOV, (float)w / (float)h, 0.1f, 100.f);
    PushMatrix();
}

glm::mat4& Project::Display::GetMatrix() {
    return this->projection_matrix;
}

void Project::_FRAMEBUFFER_CALLBACK(GLFWwindow* w, int width, int height) {
    display.SuggestDimensions();
}