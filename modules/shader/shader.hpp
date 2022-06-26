#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <generic/idable.hpp>

namespace Project {
    /**
     * @brief A shader that can be attached to a program
     * 
     */
    class Shader : public Identifiable<unsigned int> {
    private:
        /**
         * @brief compile the given shader.
         * 
         * @param src the source for the shader
         * @return true the shader compilation was successful.
         * @return false some error occured when compiling and the error should be
         * outputted to the console by stdout
         */
        bool CompileShader(const char* src);

        /**
         * @brief get the source and compile the shader.
         * 
         * @param path path to the shader
         */
        void GenShader(const char* path);

    public:
        /**
         * @brief Construct a new Shader object, compiling it to be a vertex shader or a fragment shader
         * based on the given type. type = 0 implies vertex, and type = 1 implies fragment.
         * 
         * @param path the path to the source
         * @param type the type of shader provided
         */
        Shader(const char* path, bool type);
    };
}