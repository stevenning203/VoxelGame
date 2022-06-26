#include "timer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Project::Timer::Timer() {
    this->last_time = 0.0;
    this->delta_time = 0.0;
}

void Project::Timer::Update() {
    delta_time = glfwGetTime() - last_time;
    last_time = glfwGetTime();
}

double Project::Timer::GetDeltaTime() {
    return this->delta_time;
}

void Project::Timer::Sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}