#pragma once

namespace Project {
    class ChunkMeshManager;
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
        void RenderChunkMeshManager(ChunkMeshManager& cm, Program& p, TextureAtlas& atlas);
    };
}