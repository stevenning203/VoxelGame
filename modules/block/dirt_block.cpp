#include "dirt_block.hpp"

bool Project::DirtBlock::IsOpaque() {
    return true;
}

bool Project::DirtBlock::SkipRender() {
    return false;
}

Project::DirtBlock::DirtBlock() {
    
}

int Project::DirtBlock::GetID() {
    return 2;
}