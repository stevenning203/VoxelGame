#include "block.hpp"
#include <item/item.hpp>

bool Project::Block::IsSelectable() {
    return true;
}

bool Project::Block::IsOpaque() {
    return true;
}

bool Project::Block::SkipRender() {
    return false;
}

bool Project::Block::IsSolid() {
    return true;
}

Project::Item::ToolTypeEnum Project::Block::RequiredTool() {
    return Item::ToolTypeEnum::NONE;
}

float Project::Block::Hardness() {
    return 1.f;
}