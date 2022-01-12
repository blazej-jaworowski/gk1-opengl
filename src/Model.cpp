#include "Model.h"

#include <iostream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "Shader.h"

Model::Model(float *vertices, int vertex_count, int *faces, int face_count,
             std::string vertex_filename, std::string shader_filename) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vertex_count, vertices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    link_shaders(vertex_filename, shader_filename);
}

void Model::link_shaders(std::string vertex_filename,
                         std::string shader_filename) {
    Shader vertex_shader("vertex_shader.vert", GL_VERTEX_SHADER);
    Shader fragment_shader("fragment_shader.frag", GL_FRAGMENT_SHADER);

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader.get_id());
    glAttachShader(shader_program, fragment_shader.get_id());
    glLinkProgram(shader_program);

    int success;
    char info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::cerr << "ERROR: Failed to link shader program\n"
                  << info_log << '\n';
    }

    glDeleteShader(vertex_shader.get_id());
    glDeleteShader(fragment_shader.get_id());
}

void Model::draw() {
    glBindVertexArray(vao);
    glUseProgram(shader_program);
    glDrawArrays(GL_TRIANGLES, 0, 3);
};