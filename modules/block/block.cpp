#include "block.hpp"
#include <item/item.hpp>

bool Project::Block::IsSelectable() const {
    return true;
}

bool Project::Block::IsOpaque() const {
    return true;
}

bool Project::Block::SkipRender() const {
    return false;
}

bool Project::Block::IsSolid() const {
    return true;
}

Project::Item::ToolTypeEnum Project::Block::RequiredTool() const {
    return Item::ToolTypeEnum::NONE;
}

float Project::Block::Hardness() const {
    return 1.f;
}