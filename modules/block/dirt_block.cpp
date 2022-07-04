#include "dirt_block.hpp"

bool Project::DirtBlock::IsOpaque() {
    return true;
}

bool Project::DirtBlock::SkipRender() {
    return false;
}

Project::DirtBlock::DirtBlock() {
    this->id = 2;
}