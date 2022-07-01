#pragma once
#include <generic/idable.hpp>

namespace Project {
    /**
     * @brief a 1024x1024 texture atlas for use by the chunk renderer...
     * 
     */
    class TextureAtlas : protected Identifiable<unsigned int> {
    public:
        /**
         * @brief Construct a new Texture Atlas object from the given path
         * 
         * @param path path to png
         */
        TextureAtlas(const char * path);

        /**
         * @brief bind the texture associated with the atlas
         */
        void Bind();
    };
}