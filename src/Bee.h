#pragma once

#include "Model.h"

class Bee {
    Model *bee_model;

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 target;

  public:
    Bee();
    Bee(Model *bee_model);
    void update(float dt);
    void update_model();
};