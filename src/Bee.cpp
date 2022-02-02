#include "Bee.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

Bee::Bee() {}

Bee::Bee(Model *bee_model, Model *destination_model)
    : bee_model(bee_model), destination_model(destination_model),
      position(0, 0, 3), velocity(0, 1, 1), target{} {
    set_destination(glm::vec3(10, 0, 3), 10);
    update_model();
}

void Bee::update_target(float dt) {
    if (time_until_destination < dt) {
        target = destination;
        return;
    }
    target = target + (destination - target) * (dt / time_until_destination);
    time_until_destination -= dt;
}

void Bee::update_physics(float dt) {
    glm::vec3 acceleration = (target - position);
    velocity += acceleration * dt;
    if (glm::length(velocity) > 2) {
        velocity = glm::normalize(velocity) * 2.0f;
    }
    position += velocity * dt;
}

void Bee::update(float dt) {
    update_target(dt);
    update_physics(dt);
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

void Bee::set_destination(glm::vec3 dest, float time) {
    if (time < 0) {
        time = glm::distance(dest, position);
    }
    destination = dest;
    time_until_destination = time;
    destination_model->set_position(dest);
}

glm::vec3 Bee::get_destination() { return destination; }

const glm::vec3 &Bee::get_position() const { return position; }

const glm::vec3 &Bee::get_velocity() const { return velocity; }