#pragma once

#include <generic/workable.hpp>
#include <vector>

namespace Project {
    class ChunkManager;
    class Program;
    class TextureAtlas;
    class HudElement;

    /**
     * @brief Renderer for the world
     * 
     */
    class WorldRenderer : public Workable {
    private:
        TextureAtlas* block_atlas;
        Program* shader;
        ChunkManager* chunk_manager;
        std::vector<HudElement*> hud_elements;

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

        /**
         * @brief draw the crosshair in the center of the screen
         * 
         */
        void RenderCrosshair();
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