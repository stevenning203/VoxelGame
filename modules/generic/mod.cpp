#include "mod.hpp"

int Mod(int a, int b) {
    return (a % b + b) % b;
}

int FloorDiv(int a, int b) {
    return a / b - ((a < 0 && b > 0) || (a > 0 && b < 0)) ? 1 : 0;
}