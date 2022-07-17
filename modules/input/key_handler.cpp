#include "key_handler.hpp"
#include <display/display.hpp>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <queue>

std::unordered_map<int, int> keyboard_state;
std::queue<int> key_reset_queue;
std::mutex key_mutex;

bool Project::KeyHandler::GetKeyState(int key, int action) {
    std::lock_guard<std::mutex> lock(key_mutex);
    if (!keyboard_state.count(key)) {
        return false;
    }
    return keyboard_state[key] & action;
}

Project::KeyHandler::KeyHandler(Display& d) {
    glfwSetKeyCallback(d.GetWindowPointer(), KeyHandlerGLFWCallback);
}

void Project::KeyHandler::Update() {
    std::lock_guard<std::mutex> lock(key_mutex);
    while (!key_reset_queue.empty()) {
        int front = key_reset_queue.front();
        key_reset_queue.pop();
        keyboard_state[front] &= 0b001;
    }
}

void Project::KeyHandlerGLFWCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    std::lock_guard<std::mutex> lock(key_mutex);
    if (keyboard_state.count(key) == 0) {
        keyboard_state[key] = 0;
    }
    int& c = keyboard_state[key];

    switch (action) {
        case GLFW_PRESS:
            c |= 0b101;
            key_reset_queue.push(key);
            break;
        case GLFW_RELEASE:
            c |= 0b010;
            c &= 0b110;
            key_reset_queue.push(key);
            break;
    }
}