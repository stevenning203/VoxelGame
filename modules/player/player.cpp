#include "player.hpp"
#include <generic/input_subject.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <camera/camera.hpp>

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
    if (input.GetCode() == GLFW_KEY_W) {

    } else if (input.GetCode() == GLFW_KEY_S) {

    } else if (input.GetCode() == GLFW_KEY_D) {

    } else if (input.GetCode() == GLFW_KEY_A) {
        
    }
}