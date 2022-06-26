#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <generic/idable.hpp>
#include <shader/shader.hpp>
#include <camera/camera.hpp>
#include <display/display.hpp>
#include "program.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <unordered_map>
#include <string>

Project::Program::Program(Shader& vertex_shader, Shader& fragment_shader) {
    this->id = glCreateProgram();
    glAttachShader(this->id, vertex_shader.GetID());
    glAttachShader(this->id, fragment_shader.GetID());
    glLinkProgram(this->id);
    int success;
    char log[512];
    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->id, 512, NULL, log);
        std::cout << "Shader was unable to link. Error.\n" << log << std::endl;
    }
    glDeleteShader(vertex_shader.GetID());
    glDeleteShader(fragment_shader.GetID());
}

/**
 * @brief use this program as the active program.
 * 
 */
void Project::Program::Use() {
    glUseProgram(this->id);
}

/**
 * @brief pass the given matrix to the uniform locations specified by the name
 * 
 * @param name the name of the uniform variable in the shader
 * @param data the matrix data
 */
void Project::Program::UniformMatrix(const std::string& name, const glm::mat4& data) {
    int location;
    if (!uniform_locations.count(name)) {
        location = glGetUniformLocation(this->id, name.c_str());
        uniform_locations[name] = location;
    } else {
        location = uniform_locations[name];
    }
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(data));
}

/**
 * @brief push the calculated mvp matrix to the shader
 * 
 * @param camera the camera with the view matrix that should be used
 * @param d the display
 * @param model the model matrix
 */
void Project::Program::PushMVPMatrix(Camera& camera, Display& display, glm::mat4& model) {
    glm::mat4 matrix = display.GetMatrix() * camera.GetMatrix() * model;
    this->UniformMatrix("mvp_matrix", matrix);
}