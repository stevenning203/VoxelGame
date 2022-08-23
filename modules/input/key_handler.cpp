#include "key_handler.hpp"
#include <display/display.hpp>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <queue>
#include <shared_mutex>

std::unordered_map<int, int> keyboard_state;
std::queue<int> key_reset_queue;
std::shared_mutex key_mutex;

bool Project::KeyHandler::GetKeyState(int key, int action) {
    std::shared_lock lock(key_mutex);
    if (!keyboard_state.count(key)) {
        return false;
    }
    return keyboard_state[key] & action;
}

Project::KeyHandler::KeyHandler() {
    glfwSetKeyCallback(Display::GetInstance().GetWindowPointer(), KeyHandlerGLFWCallback);
}

void Project::KeyHandler::Update() {
    std::scoped_lock lock(key_mutex);
    while (!key_reset_queue.empty()) {
        int front = key_reset_queue.front();
        key_reset_queue.pop();
        keyboard_state[front] &= 0b001;
    }
}

void Project::KeyHandler::ThreadWork() {
    
}

void Project::KeyHandler::MainThreadWork() {
    this->Update();
}

void Project::KeyHandler::Notify(const Input& input) {
    if (input.GetType() != Input::InputEnum::EVENT_KEYBOARD_STROKE) {
        return;
    }
    this->NotifyObservers(input);
}