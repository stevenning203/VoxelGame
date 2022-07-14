#include "chunk_mesh_manager.hpp"
#include "chunk_mesh.hpp"
#include "custom_pair_hasher.hpp"
#include "chunk_manager.hpp"
#include <generic/debug.hpp>
#include <string>
#include <iostream>
#include "chunk.hpp"
#include <generic/triple.hpp>

std::unordered_map<std::pair<int, int>, Project::ChunkMesh*, Project::CustomChunkPairHasher>::iterator Project::ChunkMeshManager::begin() {
    return this->meshes.begin();
}

std::unordered_map<std::pair<int, int>, Project::ChunkMesh*, Project::CustomChunkPairHasher>::iterator Project::ChunkMeshManager::end() {
    return this->meshes.end();
}

Project::ChunkMesh*& Project::ChunkMeshManager::operator()(const int row, const int col) {
    if (!this->meshes.count({row, col})) {
        throw std::runtime_error("chunk mesh manager did not contain");
    }
    return this->meshes[{row, col}];
}

void Project::ChunkMeshManager::ReMeshFlaggedMeshes() {
    for (auto& cm : *this) {
        if (cm.second->NeedsRemeshing()) {
            cm.second->ReMesh();
        }
    }
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
    this->meshes.insert({{top.First(), top.Second()}, mesh});
    this->chunk_meshing_queue.Pop();
}

Project::ChunkMeshManager::ChunkMeshManager(ChunkManager* p) : partner(p) {

}