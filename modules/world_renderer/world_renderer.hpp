#pragma once

namespace Project {
    class ChunkManager;
    class Program;

    /**
     * @brief Renderer for the world
     * 
     */
    class WorldRenderer {
    public:
        void RenderChunkManager(ChunkManager& cm, Program& p);
    };
}