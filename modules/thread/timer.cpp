#include "timer.hpp"

double Project::Timer::GetDeltaSeconds() {
    return this->delta_time;
}

void Project::Timer::Update() {
    if (delta_time == 0.0) {
        time_point = ntd::Now();
        delta_time = 0.000000001;
    } else {
        ntd::tp now = ntd::Now();
        delta_time = ntd::DurCast(now - time_point);
        time_point = now;
    }
}