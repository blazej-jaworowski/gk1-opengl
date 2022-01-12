#pragma once

#include <cstdint>
#include <string>

class Model {
    uint32_t vbo;
    uint32_t vao;
    uint32_t shader_program;

    void link_shaders(std::string vertex_filename, std::string shader_filename);

  public:
    Model(float *vertices, int vertex_count, int *faces, int face_count,
          std::string vertex_filename, std::string shader_filename);
    void draw();
};