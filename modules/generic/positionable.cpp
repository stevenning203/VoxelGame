#include <glm/glm.hpp>
#include "positionable.hpp"
#include <input/timer.hpp>

const glm::vec3& Project::Positionable::GetPosition() const {
    return this->position;
}

const glm::vec3& Project::Positionable::GetDirection() const {
    return this->direction;
}

const glm::vec3& Project::Positionable::GetVelocity() const {
    return this->velocity;
}

void Project::Positionable::AddVelocity() {
    this->position += this->velocity;
}

void Project::Positionable::AddVelocity(const float mul) {
    this->position += mul * this->velocity;
}