#include "camera.hpp"
#include <shader/program.hpp>
#include <input/mouse_handler.hpp>
#include <input/timer.hpp>
#include <generic/positionable.hpp>
#include <input/key_handler.hpp>
#include <generic/debug.hpp>

Project::Camera::Camera(Program* shader, MouseHandler* mouse, KeyHandler* keyboard, Timer* timer) : timer(timer), shader(shader), mouse(mouse), keyboard(keyboard), yaw(0.f), pitch(0.f), sensitivity(INITIAL_SENSITIVITY), speed(INITIAL_SPEED), changed(false) {
    this->position = glm::vec3{0.f, 15.f, 0.f};
}

void Project::Camera::MainThreadWork() {
    this->UpdateMovement();
    this->UpdatePanning();
    this->PushMatrix();
}

void Project::Camera::ThreadWork() {
    
}

void Project::Camera::PushMatrix() {
    std::shared_lock lock{this->mutex};
    this->shader->UniformMatrix("view_matrix", this->matrix);
}

void Project::Camera::UpdateMovement() {
    std::scoped_lock lock{this->mutex};
    if (keyboard->GetKeyState(GLFW_KEY_W, 1)) {
        this->position += this->forward * (float)timer->GetDeltaTime() * speed;
    }
    if (keyboard->GetKeyState(GLFW_KEY_A, 1)) {
        this->position += this->right * (float)timer->GetDeltaTime() * -speed;
    }
    if (keyboard->GetKeyState(GLFW_KEY_S, 1)) {
        this->position += this->forward * (float)timer->GetDeltaTime() * -speed;
    }
    if (keyboard->GetKeyState(GLFW_KEY_D, 1)) {
        this->position += this->right * (float)timer->GetDeltaTime() * speed;
    }
}

void Project::Camera::UpdatePanning() {
    int dx = mouse->MouseDX();
    int dy = mouse->MouseDY();
    float dt = static_cast<float>(timer->GetDeltaTime());
    float mult = dt * this->sensitivity;
    this->yaw += static_cast<float>(dx) * mult;
    this->pitch -= static_cast<float>(dy) * mult;
    if (this->pitch > 89.75f) {
        this->pitch = 89.75f;
    } else if (this->pitch < -89.75f) {
        this->pitch = -89.75f;
    }
    std::scoped_lock lock{this->mutex};
    this->forward = glm::normalize(
        glm::vec3(
            glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
            glm::sin(glm::radians(pitch)),
            glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))
        )
    );
    this->right = glm::normalize(glm::cross(this->forward, glm::vec3(0.f, 1.f, 0.f)));
    this->up = glm::normalize(glm::cross(this->right, this->forward));
    this->matrix = glm::lookAt(this->position, this->position + this->forward, this->up);
}

const glm::vec3& Project::Camera::GetForward() const {
    std::shared_lock lock{this->mutex};
    return this->forward;
}

const glm::vec3& Project::Camera::GetPosition() const {
    std::shared_lock lock{this->mutex};
    return this->position;
}

const glm::vec3& Project::Camera::GetForward() const {
    std::shared_lock lock(this->mutex);
    return this->right;
}