#include "worker_thread.hpp"
#include <thread/timer.hpp>

void Project::WorkerThread::AddWorkable(Workable* workable) {
    this->jobs.push_back(workable);
}

double Project::WorkerThread::GetDeltaTimeSeconds() {
    return this->timer->GetDeltaSeconds();
}

void Project::WorkerThread::Start() {
    while (1) {
        
    }
}

Project::WorkerThread::WorkerThread() {
    this->timer = new Timer();
}