#pragma once

#include <cstdint>
#include <string>

#include <glm/glm.hpp>

#include "structs.h"

class Model {
    uint32_t vbo;
    uint32_t vao;
    uint32_t ebo;
    uint32_t shader_program;
    int element_count;
    glm::vec3 position;
    glm::mat4 rotation;
    glm::vec3 model_scale;

    void set_mat4_uniform(std::string name, glm::mat4 value);
    void set_vec3_uniform(std::string name, glm::vec3 value);
    void set_float_uniform(std::string name, float value);

  public:
    Model(float *vertices, float *normals, int vertex_count, uint32_t *faces,
          int face_count, std::string vertex_filename,
          std::string fragnent_filename);
    void link_shaders(std::string vertex_filename, std::string shader_filename);
    void set_reflection_model(bool blinn);
    void draw();
    void set_model_matrix(glm::mat4 model_matrix);
    void set_view_matrix(glm::mat4 view_matrix);
    void set_projection_matrix(glm::mat4 projection_matrix);
    void set_material(Material material);
    void set_dir_light(DirLight sun_light, int index);
    void set_spot_light(SpotLight spot_light, int index);
    void update_model_matrix();
    void translate(glm::vec3 v);
    void set_position(glm::vec3 position);
    void rotate(float angle, glm::vec3 v);
    void set_rotation(glm::mat4 r);
    void scale(float s);
    void scale(glm::vec3 s);
};