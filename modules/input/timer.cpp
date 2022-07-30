#include "timer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <iostream>

Project::Timer::Timer() : delta_ns_high_res(-1) {
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

void Project::Timer::Sleep(int us) {
    std::this_thread::sleep_for(std::chrono::nanoseconds(us));
}

void Project::Timer::HighResolutionMeasureFirst() {
    this->point = std::chrono::high_resolution_clock::now();
}

void Project::Timer::FPSSleep() {

}

void Project::Timer::HighResolutionMeasureSecond() {
    long long x = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - this->point).count();
    if (x >= (long long)INT_MAX) {
        std::cout << "Nanoseconds greater than INT_MAX" << std::endl;
        return;
    }
    this->delta_ns_high_res = static_cast<int>(x);
}

int Project::Timer::GetDeltaHighResolution() {
    return this->delta_ns_high_res;
}

void Project::Timer::MainThreadWork() {
    this->Update();
    this->FPSSleep();
}

void Project::Timer::ThreadWork() {
    
}