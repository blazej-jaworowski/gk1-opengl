#pragma once

#include <fstream>
#include <string>
#include <tuple>
#include <vector>

class ObjLoader {
  public:
    int vertex_count;
    float *vertices;
    float *normals;
    int face_count;
    uint32_t *faces;

    ObjLoader(std::string filename);
    ~ObjLoader();
};