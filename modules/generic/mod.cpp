#include "mod.hpp"
#include <chunk/chunk.hpp>

int Mod(int a, int b) {
    return (a % b + b) % b;
}

int FloorDiv(int a, int b) {
    return a / b - ((((a < 0 && b > 0) || (a > 0 && b < 0)) && Mod(a, b) != 0) ? 1 : 0);
}

int ChunkMod(int x) {
    return Mod(x, Project::Chunk::CHUNK_SIZE);
}

int ChunkFloor(int x) {
    return FloorDiv(x, Project::Chunk::CHUNK_SIZE);
}