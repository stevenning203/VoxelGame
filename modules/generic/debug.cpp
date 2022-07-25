#include "debug.hpp"
#include <iostream>

void ntd::OutVector(const char * text, const glm::vec3& vec) {
    std::cout << text << ' ' << vec[0] << ' ' << vec[1] << ' ' << vec[2] << std::endl;
}

int ntd::DurCast(const ns& t) {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(t).count();
}

ntd::tp ntd::Now() {
    return spd::now();
}

void ntd::Spd(const ns& t) {
    std::cout << DurCast(t) << std::endl;
}