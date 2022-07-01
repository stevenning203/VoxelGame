#pragma once

namespace Project {
    class ChunkManager;
    class Program;
    class TextureAtlas;

    /**
     * @brief Renderer for the world
     * 
     */
    class WorldRenderer {
    public:
        /**
         * @brief Render the chunk manager with the given shaderprogram
         * 
         * @param cm chunk
         * @param p program
         */
        void RenderChunkManager(ChunkManager& cm, Program& p, TextureAtlas& atlas);
    };
}