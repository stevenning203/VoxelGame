#pragma once

#include <utility>

namespace Project {
    class CustomChunkPairHasher {
    public:
        std::size_t operator()(const std::pair<int, int>& pair) const;
    };
}