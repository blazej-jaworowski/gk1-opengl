#include "Shader.h"

#include <fstream>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

Shader::Shader(std::string filename, uint32_t type) : type(type) {
    std::ifstream file(filename);
    if (file.fail()) {
        std::cerr << "ERROR: File " << filename << " not found\n";
    }

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();

    char *source = new char[size + 1];
    file.seekg(0);
    file.read(source, size);
    source[size] = 0;

    id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    delete[] source;

    int success;
    char info_log[512];

    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, info_log);
        std::cerr << "ERROR: Shader compilation failed\n" << info_log << '\n';
    }
}

uint32_t Shader::get_id() const { return id; }