#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <generic/idable.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <unordered_map>
#include <string>

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
        std::unordered_map<std::string, int> uniform_locations;

        /**
         * @brief Get the uniform location based on the given name
         * 
         * @param s location variable name
         * @return int the location
         */
        int GetUniformLocation(const std::string& s);
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
        void UniformMatrix(const std::string& name, const glm::mat4& data);

        /**
         * @brief pass the given float to the uniform location specified by the name
         * 
         * @param name the name of the uniform variable in the program
         * @param data  the float data
         */
        void UniformFloat(const std::string& name, const float data);

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