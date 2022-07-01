#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <generic/idable.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <unordered_map>

namespace Project {
    class Shader;
    class Display;
    class Camera;

    /**
     * @brief A shader program with a vertex and a fragment shader.
     * 
     */
    class Program : public Identifiable<unsigned int> {
    private:
        /**
         * @brief hasher for c-style strings
         * 
         */
        class CustomCStringHasher {
            /**
             * @brief hash a c-style string
             * 
             * @param cstring the c-style string
             * @return std::size_t the hash
             */
            std::size_t operator()(const char * cstring) const;
        };

        std::unordered_map<const char *, int> uniform_locations;

        /**
         * @brief Get the uniform location based on the given name
         * 
         * @param s location variable name
         * @return int the location
         */
        int GetUniformLocation(const char * s);
    public:
        /**
         * @brief Construct a new Program object with the given fragment shader and vertex shader
         * 
         * @param vertex_shader the vertex shader
         * @param fragment_shader the fragment shader
         */
        Program(Shader& vertex_shader, Shader& fragment_shader);

        /**
         * @brief use this program as the active program.
         * 
         */
        void Use();

        /**
         * @brief pass the given matrix to the uniform locations specified by the name
         * 
         * @param name the name of the uniform variable in the shader
         * @param data the matrix data
         * 
         */
        void UniformMatrix(const char * name, const glm::mat4& data);

        /**
         * @brief pass the given float to the uniform location specified by the name
         * 
         * @param name the name of the uniform variable in the program
         * @param data  the float data
         */
        void UniformFloat(const char * name, const float data);

        /**
         * @brief push the calculated mvp matrix to the shader
         * 
         * @param camera the camera with the view matrix that should be used
         * @param display the display
         * @param model the model matrix
         */
        void PushMVPMatrix(Camera& camera, Display& display, glm::mat4& model);
    };
}