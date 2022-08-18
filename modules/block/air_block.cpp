#include "air_block.hpp"

bool Project::AirBlock::IsOpaque() const {
    return false;
}

bool Project::AirBlock::SkipRender() const {
    return true;
}

Project::AirBlock::AirBlock() {
    
}

int Project::AirBlock::GetID() const {
    return 0;
}

bool Project::AirBlock::IsSelectable() const {
    return false;
}

bool Project::AirBlock::IsSolid() const {
    return false;
}