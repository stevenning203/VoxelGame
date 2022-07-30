#include "block.hpp"

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