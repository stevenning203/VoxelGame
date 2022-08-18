#include "item.hpp"
#include <game/game.hpp>

void Project::Item::Use(Game& g) {
    
}

Project::Item::ToolTypeEnum Project::Item::ToolType() const {
    return ToolTypeEnum::NONE;
}

float Project::Item::BreakingPower() const {
    return 1.f;
}