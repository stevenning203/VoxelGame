#include <glm/glm.hpp>
#include "positionable.hpp"

glm::vec3& Project::Positionable::GetPosition() {
    return this->position;
}