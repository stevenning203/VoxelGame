#include "custom_pair_hasher.hpp"
#include <utility>
#include <functional>

std::size_t Project::CustomChunkPairHasher::operator()(const std::pair<int, int>& pair) const {
    static std::hash<int> x = std::hash<int>();
    return x(pair.first) ^ x(pair.second);
}