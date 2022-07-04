#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include <generic/idable.hpp>

bool Project::Shader::CompileShader(const char* src) {
    glShaderSource(this->id, 1, &src, NULL);
    glCompileShader(this->id);
    char log[512];
    int success;
    glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(this->id, 512, NULL, log);
        std::cout << "Shader compilation failed:\n" << log << std::endl;
        return false;
    }
    return true;
}

void Project::Shader::GenShader(const char* path) {
    std::string buffer;
    std::ifstream source(path);
    std::stringstream stream;
    try {
        stream << source.rdbuf();
        source.close();
        buffer = stream.str();
    } catch (std::ifstream::failure& e) {
        std::cout << "Shader error: unable to load file at given path: " << path << std::endl;
    }
    if (!CompileShader(buffer.c_str())) {
        throw std::runtime_error("Unable to compile shader. Error.");
    }
}

Project::Shader::Shader(const char* path, bool type) {
    this->id = glCreateShader((type == 0) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
    GenShader(path);
}