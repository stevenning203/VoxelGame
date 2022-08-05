#include "glm_helper.hpp"

const glm::mat4& ntd::IdentityMat4() {
    static constexpr glm::mat4 identity(1.f);
    return identity;
}