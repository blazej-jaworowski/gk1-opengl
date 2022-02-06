#include "Model.h"

#include <iostream>

#include <GL/glew.h>
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
    : element_count(face_count * 3), position(0.0f), rotation(1.0f),
      model_scale(1.0f) {
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

    update_model_matrix();
}

void Model::link_shaders(std::string vertex_filename,
                         std::string fragment_filename) {
    Shader vertex_shader(vertex_filename, GL_VERTEX_SHADER);
    Shader fragment_shader(fragment_filename, GL_FRAGMENT_SHADER);

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader.get_id());
    glAttachShader(shader_program, fragment_shader.get_id());
    glLinkProgram(shader_program);

    int success;
    char info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::cerr << "ERROR(" << vertex_filename << ", " << fragment_filename
                  << "): Failed to link shader program\n"
                  << info_log << '\n';
        exit(1);
    }

    glDeleteShader(vertex_shader.get_id());
    glDeleteShader(fragment_shader.get_id());
}

void Model::set_mat4_uniform(std::string name, glm::mat4 value) {
    glUseProgram(shader_program);
    int32_t location = glGetUniformLocation(shader_program, name.c_str());
    if (location == -1) {
        // std::cerr << "ERROR: Uniform " << name << " not found\n";
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Model::update_model_matrix() {
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), model_scale);
    Model::set_model_matrix(translation_matrix * rotation * scale_matrix);
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
        // std::cerr << "ERROR: Uniform " << name << " not found\n";
        return;
    }
    glUniform3f(location, value.x, value.y, value.z);
}

void Model::set_float_uniform(std::string name, float value) {
    glUseProgram(shader_program);
    int32_t location = glGetUniformLocation(shader_program, name.c_str());
    if (location == -1) {
        // std::cerr << "ERROR: Uniform " << name << " not found\n";
        return;
    }
    glUniform1f(location, value);
}

void Model::set_material(Material material) {
    this->material = material;
    set_vec3_uniform("material.ambient", material.ambient);
    set_vec3_uniform("material.diffuse", material.diffuse);
    set_vec3_uniform("material.specular", material.specular);
    set_float_uniform("material.shininess", material.shininess);
}

void Model::set_dir_light(DirLight dir_light, int index) {
    set_vec3_uniform("dir_lights[" + std::to_string(index) + "].ambient",
                     dir_light.ambient);
    set_vec3_uniform("dir_lights[" + std::to_string(index) + "].diffuse",
                     dir_light.diffuse);
    set_vec3_uniform("dir_lights[" + std::to_string(index) + "].specular",
                     dir_light.specular);
    set_vec3_uniform("dir_lights[" + std::to_string(index) + "].direction",
                     dir_light.direction);
}

void Model::draw() {
    glUseProgram(shader_program);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, element_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Model::translate(glm::vec3 v) {
    position += v;
    update_model_matrix();
}

void Model::rotate(float angle, glm::vec3 v) {
    rotation = glm::rotate(rotation, angle, v);
    update_model_matrix();
}

void Model::set_rotation(glm::mat4 r) {
    rotation = r;
    update_model_matrix();
}

void Model::scale(glm::vec3 s) {
    model_scale *= s;
    update_model_matrix();
}

void Model::scale(float s) {
    model_scale *= glm::vec3(s);
    update_model_matrix();
}

void Model::set_bool_uniform(std::string name, bool value) {
    glUseProgram(shader_program);
    int32_t location = glGetUniformLocation(shader_program, name.c_str());
    if (location == -1) {
        // std::cerr << "ERROR: Uniform " << name << " not found\n";
        return;
    }
    glUniform1i(location, value);
}

void Model::set_reflection_model(bool blinn) {
    set_bool_uniform("blinn_reflection", blinn);
}

void Model::set_position(glm::vec3 position) {
    this->position = position;
    update_model_matrix();
}

void Model::set_spot_light(SpotLight spot_light, int index) {
    set_vec3_uniform("spot_lights[" + std::to_string(index) + "].ambient",
                     spot_light.ambient);
    set_vec3_uniform("spot_lights[" + std::to_string(index) + "].diffuse",
                     spot_light.diffuse);
    set_vec3_uniform("spot_lights[" + std::to_string(index) + "].specular",
                     spot_light.specular);
    set_float_uniform("spot_lights[" + std::to_string(index) + "].constant",
                      spot_light.constant);
    set_float_uniform("spot_lights[" + std::to_string(index) + "].linear",
                      spot_light.linear);
    set_float_uniform("spot_lights[" + std::to_string(index) + "].quadratic",
                      spot_light.quadratic);
    set_vec3_uniform("spot_lights[" + std::to_string(index) + "].position",
                     spot_light.position);
    set_vec3_uniform("spot_lights[" + std::to_string(index) + "].direction",
                     spot_light.direction);
    set_float_uniform("spot_lights[" + std::to_string(index) + "].cutoff",
                      spot_light.cutoff);
}

void Model::set_fog_dist(float fog_dist) {
    set_float_uniform("fog_dist", fog_dist);
}

void Model::set_fog_enabled(bool fog_enabled) {
    set_bool_uniform("fog_enabled", fog_enabled);
}

void Model::set_fog_color(glm::vec3 fog_color) {
    set_vec3_uniform("fog_color", fog_color);
}

void Model::reset_material() { set_material(this->material); }