#include "player.hpp"
#include <generic/input_subject.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <camera/camera.hpp>
#include <iostream>
#include <input/timer.hpp>

void Project::Player::SetPosition(const float x, const float y, const float z) {
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
}

void Project::Player::SetPosition(const glm::vec3& v) {
    this->position.operator=(v);
}

void Project::Player::SetDirection(const float x, const float y, const float z) {
    this->direction.x = x;
    this->direction.y = y;
    this->direction.z = z;
}

const glm::vec3& Project::Player::GetDimensions() const {
    return PLAYER_DIMENSIONS;
}

void Project::Player::SetDirection(const glm::vec3& v) {
    this->direction.operator=(v);
}

void Project::Player::SetVelocity(const float x, const float y, const float z) {
    this->velocity.x = x;
    this->velocity.y = y;
    this->velocity.z = z;
}

void Project::Player::SetVelocity(const glm::vec3& v) {
    this->velocity.operator=(v);
}


Project::Item* Project::Player::InHand() {
    return this->in_hand;
}

void Project::Player::Notify(const Input& input) {
    if (input.GetType() != Input::InputEnum::EVENT_KEYBOARD_STROKE) {
        return;
    }
    this->velocity[0] = 0.f;
    this->velocity[1] = 0.f;
    this->velocity[2] = 0.f;
    if (input.GetAction() == GLFW_PRESS) {
        if (input.GetCode() == GLFW_KEY_W) {
            this->forward = true;
        } else if (input.GetCode() == GLFW_KEY_S) {
            this->back = true;
        } else if (input.GetCode() == GLFW_KEY_D) {
            this->right = true;
        } else if (input.GetCode() == GLFW_KEY_A) {
            this->left = true;
        } else if (input.GetCode() == GLFW_KEY_SPACE) {
            // jump
        }
    } else if (input.GetAction() == GLFW_RELEASE) {
        if (input.GetCode() == GLFW_KEY_W) {
            this->forward = false;
        } else if (input.GetCode() == GLFW_KEY_S) {
            this->back = false;
        } else if (input.GetCode() == GLFW_KEY_D) {
            this->right = false;
        } else if (input.GetCode() == GLFW_KEY_A) {
            this->left = false;
        }
    }
    float constant = 1.25f;
    if (this->forward) {
        this->velocity += this->camera->GetForward() * constant;
    }
    if (this->back) {
        this->velocity -= this->camera->GetForward() * constant;
    }
    if (this->right) {
        this->velocity += this->camera->GetRight() * constant;
    }
    if (this->left) {
        this->velocity -= this->camera->GetRight() * constant;
    }
}

Project::Player::Player(Camera* camera) {
    this->camera = camera;
    this->position = {0.f, 23.f, 0.f};
}