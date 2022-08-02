#include "item.hpp"
#include <game/game.hpp>

void Project::Item::Use(Game& g) {
    
}

Project::Item::ToolTypeEnum Project::Item::ToolType() {
    return ToolTypeEnum::NONE;
}

float Project::Item::BreakingPower() {
    return 1.f;
}