#include "mouse_handler.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include <display/display.hpp>
#include <atomic>
#include <iostream>

std::atomic<bool> key_state[6]{false, false, false, false, false};

bool Project::MouseHandler::GetMouseState(MouseEnum enumer) const {
    return key_state[static_cast<int>(enumer)];
}

int Project::MouseHandler::MouseX() const {
    return this->mousex;
}

int Project::MouseHandler::MouseDX() const {
    return this->dx;
}

int Project::MouseHandler::MouseDY() const {
    return this->dy;
}

int Project::MouseHandler::MouseY() const {
    return this->mousey;
}

Project::MouseHandler::MouseHandler() {
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(Display::GetInstance().GetWindowPointer(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void Project::MouseHandler::Update() {
    double a, b;
    glfwGetCursorPos(Display::GetInstance().GetWindowPointer(), &a, &b);
    this->mousex = static_cast<int>(a);
    this->mousey = static_cast<int>(b);
    this->dx = this->mousex - this->lx;
    this->dy = this->mousey - this->ly;
    this->lx.store(this->mousex.load());
    this->ly.store(this->mousey.load());
    key_state[0] = false;
    key_state[1] = false;
    key_state[4] = false;
    key_state[5] = false;
    this->NotifyObservers(Input(Input::InputEnum::EVENT_MOUSE_MOVE, 0, 0, static_cast<int>(a), static_cast<int>(b)));
}

void Project::MouseHandler::MainThreadWork() {
    this->Update();
}

void Project::MouseHandler::ThreadWork() {
    
}

void Project::MouseHandler::Notify(const Input& input) {
    if (input.GetType() == Input::InputEnum::EVENT_KEYBOARD_STROKE) {
        return;
    }
    this->NotifyObservers(input);
}