#include "air_block.hpp"

bool Project::AirBlock::IsOpaque() {
    return false;
}

bool Project::AirBlock::SkipRender() {
    return true;
}

Project::AirBlock::AirBlock() {
    
}

int Project::AirBlock::GetID() {
    return 0;
}

bool Project::AirBlock::IsSelectable()  {
    return false;
}

bool Project::AirBlock::IsSolid() {
    return false;
}