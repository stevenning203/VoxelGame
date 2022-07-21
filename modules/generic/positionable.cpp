#include <glm/glm.hpp>
#include "positionable.hpp"

const glm::vec3& Project::Positionable::GetPosition() {
    return this->position;
}