#include "Model.h"

#include <iostream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

float *merge_vertex_data(float *vertices, float *normals, int vertex_count) {
    float *r = new float[6 * vertex_count];
    for (int i = 0; i < vertex_count; i++) {
        r[6 * i + 0] = vertices[3 * i + 0];
        r[6 * i + 1] = vertices[3 * i + 1];
        r[6 * i + 2] = vertices[3 * i + 2];

        r[6 * i + 3] = normals[3 * i + 0];
        r[6 * i + 4] = normals[3 * i + 1];
        r[6 * i + 5] = normals[3 * i + 2];
    }
    return r;
}

Model::Model(float *vertices, float *normals, int vertex_count, uint32_t *faces,
             int face_count, std::string vertex_filename,
             std::string fragment_filename)
    : element_count(face_count * 3) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    float *vertex_data = merge_vertex_data(vertices, normals, vertex_count);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * vertex_count, vertex_data,
                 GL_STATIC_DRAW);
    delete[] vertex_data;

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*faces) * element_count, faces,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    link_shaders(vertex_filename, fragment_filename);

    set_model_matrix(glm::mat4(1.0f));
}

void Model::link_shaders(std::string vertex_filename,
                         std::string fragment_filename) {
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
    if (location == -1) {
        std::cerr << "ERROR: Uniform " << name << " not found\n";
        return;
    }
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

void Model::set_vec3_uniform(std::string name, glm::vec3 value) {
    glUseProgram(shader_program);
    int32_t location = glGetUniformLocation(shader_program, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR: Uniform " << name << " not found\n";
        return;
    }
    glUniform3f(location, value.x, value.y, value.z);
}

void Model::set_float_uniform(std::string name, float value) {
    glUseProgram(shader_program);
    int32_t location = glGetUniformLocation(shader_program, name.c_str());
    if (location == -1) {
        std::cerr << "ERROR: Uniform " << name << " not found\n";
        return;
    }
    glUniform1f(location, value);
}

void Model::set_material(Material material) {
    set_vec3_uniform("material.ambient", material.ambient);
    set_vec3_uniform("material.diffuse", material.diffuse);
    set_vec3_uniform("material.specular", material.specular);
    set_float_uniform("material.shininess", material.shininess);
}

void Model::set_sun_light(SunLight sun_light) {
    set_vec3_uniform("sun_light.ambient", sun_light.ambient);
    set_vec3_uniform("sun_light.diffuse", sun_light.diffuse);
    set_vec3_uniform("sun_light.specular", sun_light.specular);
    set_vec3_uniform("sun_light.direction", sun_light.direction);
}

void Model::draw() {
    glUseProgram(shader_program);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, element_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
};