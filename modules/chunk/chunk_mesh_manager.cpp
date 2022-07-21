#include "chunk_mesh_manager.hpp"
#include "chunk_mesh.hpp"
#include "custom_pair_hasher.hpp"
#include "chunk_manager.hpp"
#include <generic/debug.hpp>
#include <string>
#include <iostream>
#include "chunk.hpp"
#include <generic/triple.hpp>
#include <mutex>

std::unordered_map<std::pair<int, int>, Project::ChunkMesh*, Project::CustomChunkPairHasher>::iterator Project::ChunkMeshManager::begin() {
    return this->meshes.begin();
}

std::unordered_map<std::pair<int, int>, Project::ChunkMesh*, Project::CustomChunkPairHasher>::iterator Project::ChunkMeshManager::end() {
    return this->meshes.end();
}

void Project::ChunkMeshManager::ReMeshFlaggedMeshes() {
    std::unique_lock<std::mutex> lock(this->map_access_mutex);
    for (auto& cm : *this) {
        if (cm.second->NeedsRemeshing()) {
            this->chunk_remeshing_queue.Push({cm.first.first, cm.first.second});
            cm.second->ResetNeedsMeshing();
        }
    }
    if (this->chunk_remeshing_queue.Empty()) {
        return;
    }
    std::pair<int, int> top = this->chunk_remeshing_queue.Front();
    if (!this->meshes.count(top)) {
        return;
    }
    lock.unlock();
    this->meshes[top]->ReMesh();
    this->chunk_remeshing_queue.Pop();
}

void Project::ChunkMeshManager::QueueMeshGeneration(int a, int b, Chunk* y) {
    this->chunk_meshing_queue.Emplace(a, b, y);
}

void Project::ChunkMeshManager::GenerateQueuedMeshes() {
    if (this->chunk_meshing_queue.Empty()) {
        return;
    }
    ntd::Triple top = this->chunk_meshing_queue.Front();
    ChunkMesh* mesh = new ChunkMesh(top.Third());
    std::unique_lock<std::mutex> lock(this->map_access_mutex);
    this->meshes.insert({{top.First(), top.Second()}, mesh});
    lock.unlock();
    this->chunk_meshing_queue.Pop();
}

Project::ChunkMeshManager::ChunkMeshManager(ChunkManager* p) : partner(p) {

}

void Project::ChunkMeshManager::QueueRemesh(const int x, const int z) {
    std::lock_guard<std::mutex> lock(this->map_access_mutex);
    if (!this->meshes.count({x, z})) {
        return;
    }
    this->meshes[{x, z}]->SuggestReMesh();
}