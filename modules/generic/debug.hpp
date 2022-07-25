#pragma once

#include <iostream>
#include <glm/vec3.hpp>
#include <chrono>

#define ntd__Debug__Time(x) ntd::tp time_point_debug = ntd::Now(); x; ntd::Spd(ntd::Now() - time_point_debug)

namespace ntd {
    typedef std::chrono::high_resolution_clock::time_point tp;
    typedef std::chrono::nanoseconds ns;
    typedef std::chrono::high_resolution_clock spd;
    
    tp Now();

    int DurCast(const ns& t);

    void Spd(const ns& t);

    void OutVector(const char * text, const glm::vec3& v);
}