#pragma once

#include <fstream>
#include <string>
#include <tuple>
#include <vector>

class ObjLoader {
  public:
    std::vector<std::tuple<float, float, float>> vertices;
    std::vector<std::tuple<int, int, int>> faces;

    ObjLoader(std::string filename);
};