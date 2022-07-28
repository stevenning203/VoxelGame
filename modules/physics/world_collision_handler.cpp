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

void Project::WorldCollisionHandler::EnforcePlayerVoxelCollision() {
    
}

Project::WorldCollisionHandler::WorldCollisionHandler(ChunkManager* cm, Camera* c, MouseHandler* mouse) : chunk_manager(cm), camera(c), mouse(mouse) {
    
}

void Project::WorldCollisionHandler::ThreadWork() {

}

void Project::WorldCollisionHandler::MainThreadWork() {

}