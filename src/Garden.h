#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Model.h"

class Garden {
  private:
    std::vector<Model> models;

  public:
    void init(int width, int height);
    void update_camera(glm::vec3 pos, glm::vec3 looking_at);
    void update_projection(int width, int height);
    void draw();
};