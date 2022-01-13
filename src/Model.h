#pragma once

#include <cstdint>
#include <string>

#include <glm/glm.hpp>

class Model {
    uint32_t vbo;
    uint32_t vao;
    uint32_t ebo;
    uint32_t shader_program;
    int element_count;

    void link_shaders(std::string vertex_filename, std::string shader_filename);
    void set_mat4_uniform(std::string name, glm::mat4 value);

  public:
    Model(float *vertices, float *normals, int vertex_count, uint32_t *faces,
          int face_count, std::string vertex_filename,
          std::string fragnent_filename);
    void draw();
    void set_model_matrix(glm::mat4 model_matrix);
    void set_view_matrix(glm::mat4 view_matrix);
    void set_projection_matrix(glm::mat4 projection_matrix);
    void set_color(float r, float g, float b);
};