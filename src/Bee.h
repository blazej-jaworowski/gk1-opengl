#pragma once

#include "Model.h"

class Bee {
    Model *bee_model;
    Model *destination_model;

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 target;

    glm::vec3 destination;
    float time_until_destination;

    void update_model();
    void update_physics(float dt);
    void update_target(float dt);

  public:
    Bee();
    Bee(Model *bee_model, Model *destination_model);
    void update(float dt);
    void set_destination(glm::vec3 dest, float time = -1);
    glm::vec3 get_destination();
    const glm::vec3 &get_position() const;
    const glm::vec3 &get_velocity() const;
};