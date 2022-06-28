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

void Project::Program::Use() {
    glUseProgram(this->id);
}

int Project::Program::GetUniformLocation(const std::string& s) {
    int location;
    if (!uniform_locations.count(s)) {
        location = glGetUniformLocation(this->id, s.c_str());
        uniform_locations[s] = location;
    } else {
        location = uniform_locations[s];
    }
    return location;
}

void Project::Program::UniformMatrix(const std::string& name, const glm::mat4& data) {
    int location = GetUniformLocation(name);
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(data));
}

void Project::Program::UniformFloat(const std::string& name, const float data) {
    int location = GetUniformLocation(name);
    glUniform1f(location, data);
}

void Project::Program::PushMVPMatrix(Camera& camera, Display& display, glm::mat4& model) {
    glm::mat4 matrix = display.GetMatrix() * camera.GetMatrix() * model;
    this->UniformMatrix("mvp_matrix", matrix);
}