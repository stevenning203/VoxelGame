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

int Project::Program::GetUniformLocation(const char * s) {
    int location;
    if (!uniform_locations.count(s)) {
        location = glGetUniformLocation(this->id, s);
        uniform_locations[s] = location;
    } else {
        location = uniform_locations[s];
    }
    return location;
}

void Project::Program::UniformMatrix(const char * name, const glm::mat4& data) {
    int location = GetUniformLocation(name);
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(data));
}

void Project::Program::UniformBool(const char * name, const bool data) {
    int location = GetUniformLocation(name);
    glUniform1i(location, data);
}

void Project::Program::UniformFloat(const char * name, const float data) {
    int location = GetUniformLocation(name);
    glUniform1f(location, data);
}

void Project::Program::UniformVec3(const char * name, const glm::vec3& data) {
    int location = GetUniformLocation(name);
    glUniform3fv(location, 1, &data[0]);
}

std::size_t Project::Program::CustomCStringHasher::operator()(const char* cstring) const {
    size_t start = 5381;
    int inc;
    while ((inc = *cstring++)) {
        start = ((start << 5) + start) + inc;
    }
    return start;
}