#include "Bee.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

Bee::Bee() {}

Bee::Bee(Model *bee_model)
    : bee_model(bee_model), position(0, 1, 3), velocity(1.5, 0, 0),
      target(0, 0, 4) {
    update_model();
}

void Bee::update(float dt) {
    glm::vec3 acceleration = (target - position);
    velocity += acceleration * dt;
    if (glm::length(velocity) > 1) {
        velocity = glm::normalize(velocity);
    }
    position += velocity * dt;
    update_model();
}

void Bee::update_model() {
    glm::vec3 c = glm::cross(glm::vec3(0, 0, 1), glm::normalize(velocity));
    if (c == glm::vec3(0)) {
        c = glm::vec3(1, 0, 0);
    }
    float angle = std::asin(glm::length(c));
    if (std::isnan(angle)) {
        angle = M_PI_2;
    }
    if (velocity.z < 0) {
        angle = M_PI - angle;
    }
    glm::vec2 vel_proj(velocity.x, velocity.y);
    float angle_2d = std::asin(vel_proj.x / glm::length(vel_proj));
    if (vel_proj.y > 0) {
        angle_2d = M_PI - angle_2d;
    }
    glm::mat4 rotation =
        glm::rotate(glm::mat4(1), angle_2d, glm::vec3(0, 0, 1));
    rotation = glm::rotate(rotation, angle, glm::vec3(1, 0, 0));
    bee_model->set_rotation(rotation);
    bee_model->set_position(position);
}