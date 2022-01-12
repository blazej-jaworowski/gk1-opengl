#include "Model.h"

#include <iostream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

Model::Model(float *vertices, int vertex_count, uint32_t *faces, int face_count,
             std::string vertex_filename, std::string shader_filename)
    : element_count(face_count * 3) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vertex_count, vertices,
                 GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*faces) * element_count, faces,
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

void Model::set_mat4_uniform(std::string name, glm::mat4 value) {
    glUseProgram(shader_program);
    int32_t location = glGetUniformLocation(shader_program, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Model::set_model_matrix(glm::mat4 model_matrix) {
    set_mat4_uniform("model_matrix", model_matrix);
}

void Model::set_view_matrix(glm::mat4 view_matrix) {
    set_mat4_uniform("view_matrix", view_matrix);
}

void Model::set_projection_matrix(glm::mat4 projection_matrix) {
    set_mat4_uniform("projection_matrix", projection_matrix);
}

void Model::draw() {
    glUseProgram(shader_program);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, element_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
};