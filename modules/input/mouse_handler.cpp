#include "mouse_handler.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include <display/display.hpp>

bool key_state[6]{false, false, false, false, false};

bool Project::MouseHandler::GetMouseState(MouseEnum enumer) {
    return key_state[static_cast<int>(enumer)];
}

int Project::MouseHandler::MouseX() {
    return this->mousex;
}

int Project::MouseHandler::MouseDX() {
    return this->dx;
}

int Project::MouseHandler::MouseDY() {
    return this->dy;
}

int Project::MouseHandler::MouseY() {
    return this->mousey;
}

Project::MouseHandler::MouseHandler(Display& d) {
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(d.GetWindowPointer(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSetMouseButtonCallback(d.GetWindowPointer(), MouseHandlerGLFWCallback);
}

void Project::MouseHandler::Update(Display& d) {
    double a, b;
    glfwGetCursorPos(d.GetWindowPointer(), &a, &b);
    this->mousex = static_cast<int>(a);
    this->mousey = static_cast<int>(b);
    this->dx = this->mousex - this->lx;
    this->dy = this->mousey - this->ly;
    this->lx = this->mousex;
    this->ly = this->mousey;
    for (int i{0}; i < 6; i++) {
        key_state[i] = false;
    }
}

void Project::MouseHandlerGLFWCallback(GLFWwindow* w, int b, int a, int m) {
    if (a == GLFW_PRESS) {
        switch (b) {
        case GLFW_MOUSE_BUTTON_LEFT:
            key_state[0] = true;
            key_state[2] = true;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            key_state[1] = true;
            key_state[3] = true;
            break;
        }
    } else if (a == GLFW_RELEASE) {
        switch (b) {
        case GLFW_MOUSE_BUTTON_LEFT:
            key_state[4] = true;
            key_state[2] = false;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            key_state[5] = true;
            key_state[3] = false;
            break;
        }
    }
}