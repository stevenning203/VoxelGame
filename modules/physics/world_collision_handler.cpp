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

Project::WorldCollisionHandler::WorldCollisionHandler(ChunkManager* cm, Camera* c, MouseHandler* mouse, Player* player) : player(player), chunk_manager(cm), camera(c), mouse(mouse) {
    
}

void Project::WorldCollisionHandler::UpdatePlayerCamera() {
    this->player->SetPosition(this->camera->GetPosition());
    this->player->SetDirection(this->camera->GetForward());
}

void Project::WorldCollisionHandler::ThreadWork() {
    this->UpdatePlayerCamera();
}

void Project::WorldCollisionHandler::MainThreadWork() {

}