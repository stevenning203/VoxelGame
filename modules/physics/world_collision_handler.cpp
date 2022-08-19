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
    this->caster = new DDACaster();
}

void Project::WorldCollisionHandler::UpdatePlayerCamera() {
    this->player->SetPosition(this->camera->GetPosition());
    this->player->SetDirection(this->camera->GetForward()); 
}

void Project::WorldCollisionHandler::ThreadWork() {
    this->UpdatePlayerCamera();
    this->UpdatePlayerPosition();
}

void Project::WorldCollisionHandler::MainThreadWork() {
    
}

float Project::WorldCollisionHandler::SweptAABB(const Entity& entity, const int x, const int y, const int z) {
    glm::vec3 entry_distance, exit_distance;
    glm::vec3 box_location{static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)};
    glm::vec3 box_dimensions(1.f);
    const glm::vec3& position = entity.GetPosition();
    const glm::vec3& velocity = entity.GetVelocity();
    const glm::vec3& dimensions = entity.GetDimensions();
    if (velocity.x > 0.f) {
        entry_distance.x = box_location.x - (position.x - dimensions.x);
        exit_distance.x = (box_location.x + box_dimensions.x) - dimensions.x;
    } else {
        entry_distance.x = (box_location.x + box_dimensions.x) - position.x;
        exit_distance.x = box_location.x - (position.x + dimensions.x);
    }
    if (velocity.y > 0.f) {
        entry_distance.y = box_location.y - (position.y - dimensions.y);
        exit_distance.y = (box_location.y + box_dimensions.y) - dimensions.y;
    } else {
        entry_distance.y = (box_location.y + box_dimensions.y) - position.y;
        exit_distance.y = box_location.y - (position.y + dimensions.y);
    }
    if (velocity.z > 0.f) {
        entry_distance.z = box_location.z - (position.z - dimensions.z);
        exit_distance.z = (box_location.z + box_dimensions.z) - dimensions.z;
    } else {
        entry_distance.z = (box_location.z + box_dimensions.z) - position.z;
        exit_distance.z = box_location.z - (position.z + dimensions.z);
    }
    glm::vec3 entry, exit;
    if (velocity.x == 0.f) {
        entry.x = -std::numeric_limits<float>::infinity();
        exit.x = std::numeric_limits<float>::infinity();
    } else {
        entry.x = entry_distance.x / velocity.x;
        exit.x = exit_distance.x / velocity.x;
    }

    if (velocity.y == 0.f) {
        entry.y = -std::numeric_limits<float>::infinity();
        exit.y = std::numeric_limits<float>::infinity();
    } else {
        entry.y = entry_distance.y / velocity.y;
        exit.y = exit_distance.y / velocity.y;
    }

    if (velocity.z == 0.f) {
        entry.z = -std::numeric_limits<float>::infinity();
        exit.z = std::numeric_limits<float>::infinity();
    } else {
        entry.z = entry_distance.z / velocity.z;
        exit.z = exit_distance.z / velocity.z;
    }

    float entry_time = std::max(entry.z, std::max(entry.x, entry.y));
    float exit_time = std::min(exit.z, std::min(exit.x, exit.y));

    if (entry_time > exit_time || entry.x < 0.f && entry.y < 0.f || entry.x > 1.f || entry.y > 1.f) {
        return 1.f;
    }
    return entry_time;
}

void Project::WorldCollisionHandler::UpdatePlayerPosition() {
    int hitx, hity = -1, hitz;
    int garbage0, garbage1, garbage2;
    this->caster->Cast(this->player->GetPosition(), glm::normalize(this->player->GetVelocity()), glm::length(this->player->GetVelocity()), *this->chunk_manager, hitz, hitz, hity, garbage0, garbage1, garbage2);
    if (hity == -1) { // return if the player will not collide with anything next frame
        this->player->AddVelocity();
        return;
    }
    // else...
    glm::vec3&& normal_test = player->GetPosition() - glm::vec3(hitx, hity, hitz);
    glm::vec3 normal(0.f);
    if (normal_test.x > normal_test.y && normal_test.x > normal_test.z) {
        normal.x = 1.f * glm::sign(normal_test.x);
    } else if (normal_test.y > normal_test.x && normal_test.y > normal_test.z) {
        normal.y = 1.f * glm::sign(normal_test.y);
    } else {
        normal.z = 1.f * glm::sign(normal_test.z);
    }
    float time = SweptAABB(*this->player, hitx, hity, hitz);
    if (time >= 1.f) {
        this->player->AddVelocity();
        return;
    }
    this->player->SetVelocity(this->player->GetVelocity() + this->player->GetVelocity() * normal);
    this->player->SetPosition(this->player->GetPosition() + this->player->GetVelocity() * time);
}