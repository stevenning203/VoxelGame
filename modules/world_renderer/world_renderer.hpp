#pragma once

#include <generic/workable.hpp>

namespace Project {
    class ChunkManager;
    class Program;
    class TextureAtlas;

    /**
     * @brief Renderer for the world
     * 
     */
    class WorldRenderer : public Workable {
    private:
        TextureAtlas* block_atlas;
        Program* shader;
        ChunkManager* chunk_manager;

        /**
         * @brief Render the chunk manager with the given shaderprogram
         * 
         * @param cm chunk
         * @param p program
         */
        void RenderChunkManager();

        /**
         * @brief draw the hud with all the components
         * 
         */
        void RenderHUD();
    public:

        /**
         * @brief Construct a new World Renderer object
         * 
         * @param p 
         */
        WorldRenderer(ChunkManager* cm, Program* shader);

        virtual void MainThreadWork() override;

        virtual void ThreadWork() override;
    };
}