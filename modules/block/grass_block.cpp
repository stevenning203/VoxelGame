#include "grass_block.hpp"

bool Project::GrassBlock::IsOpaque() {
    return true;
}

bool Project::GrassBlock::SkipRender() {
    return false;
}