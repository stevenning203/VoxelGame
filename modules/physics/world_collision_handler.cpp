#include "world_collision_handler.hpp"
#include <chunk/chunk_manager.hpp>
#include <player/player.hpp>
#include "dda_caster.hpp"
#include <camera/camera.hpp>
#include <block/air_block.hpp>
#include <iostream>
#include <generic/mod.hpp>
#include <chunk/chunk.hpp>
#include <input/mouse_handler.hpp>

void Project::WorldCollisionHandler::EnforcePlayerVoxelCollision(ChunkManager& cm, Player& p) {
    
}

void Project::WorldCollisionHandler::EnablePlayerBlockDestruction(ChunkManager& cm, Camera& camera, MouseHandler& mouse, float reach) {
    int r, c, y;
    int pr, pc, py;
    bool collide = this->ray_caster->Cast(camera.GetPosition(), camera.GetForward(), reach, cm, r, c, y, pr, pc, py);
    if (!collide) {
        return;
    }
    // drop blocks here?
    delete cm.operator()(r, y, c);
    cm.operator()(r, y, c) = new AirBlock();
    int row = FloorDiv(r, Chunk::CHUNK_SIZE);
    int col = FloorDiv(c, Chunk::CHUNK_SIZE);
    cm.HintRemeshing(row, col);
}

Project::WorldCollisionHandler::WorldCollisionHandler() {
    this->ray_caster = new DDACaster();
}