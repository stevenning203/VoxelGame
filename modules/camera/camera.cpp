#include "camera.hpp"
#include <shader/program.hpp>
#include <input/mouse_handler.hpp>
#include <input/timer.hpp>
#include <generic/positionable.hpp>
#include <input/key_handler.hpp>

Project::Camera::Camera() : yaw(0.f), pitch(0.f), sensitivity(INITIAL_SENSITIVITY), speed(INITIAL_SPEED), changed(false) {
    this->position = glm::vec3(0.f);
}

void Project::Camera::PushMatrix(Program& shader) {
    std::lock_guard<std::mutex> lock(this->update_mutex);
    shader.UniformMatrix("view_matrix", this->matrix);
}

void Project::Camera::UpdateMovement(KeyHandler& keyboard, Timer& t) {
    if (keyboard.GetKeyState(GLFW_KEY_W, 1)) {
        this->position += this->forward * (float)t.GetDeltaTime() * speed;
    }
    if (keyboard.GetKeyState(GLFW_KEY_A, 1)) {
        this->position += this->right * (float)t.GetDeltaTime() * -speed;
    }
    if (keyboard.GetKeyState(GLFW_KEY_S, 1)) {
        this->position += this->forward * (float)t.GetDeltaTime() * -speed;
    }
    if (keyboard.GetKeyState(GLFW_KEY_D, 1)) {
        this->position += this->right * (float)t.GetDeltaTime() * speed;
    }
}

void Project::Camera::UpdatePanning(MouseHandler& m, Timer& t) {
    int dx = m.MouseDX();
    int dy = m.MouseDY();
    float dt = static_cast<float>(t.GetDeltaTime());
    float mult = dt * this->sensitivity;
    this->yaw += static_cast<float>(dx) * mult;
    this->pitch -= static_cast<float>(dy) * mult;
    if (this->pitch > 89.5f) {
        this->pitch = 89.5f;
    } else if (this->pitch < -89.5f) {
        this->pitch = -89.5f;
    }
    this->forward = glm::normalize(
        glm::vec3(
            glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
            glm::sin(glm::radians(pitch)),
            glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))
        )
    );
    this->right = glm::normalize(glm::cross(this->forward, glm::vec3(0.f, 1.f, 0.f)));
    this->up = glm::normalize(glm::cross(this->right, this->forward));
    std::lock_guard<std::mutex> lock(this->update_mutex);
    this->matrix = glm::lookAt(this->GetPosition(), this->GetPosition() + this->forward, this->up);
}

glm::vec3 Project::Camera::GetForward() {
    return this->forward;
}

glm::vec3 Project::Camera::GetPosition() {
    return this->position;
}