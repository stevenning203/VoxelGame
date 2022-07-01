#include "grass_block.hpp"

bool Project::GrassBlock::IsOpaque() {
    return true;
}

bool Project::GrassBlock::SkipRender() {
    return false;
}

Project::GrassBlock::GrassBlock() {
    this->id = 1;
}