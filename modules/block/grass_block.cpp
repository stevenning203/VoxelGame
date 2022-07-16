#include "grass_block.hpp"

bool Project::GrassBlock::IsOpaque() {
    return true;
}

bool Project::GrassBlock::SkipRender() {
    return false;
}

Project::GrassBlock::GrassBlock() {
    
}

int Project::GrassBlock::GetID() {
    return 1;
}

bool Project::GrassBlock::IsSolid() {
    return true;
}