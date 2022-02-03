#pragma once

#include <glm/glm.hpp>

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct DirLight {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 direction;
};

struct SpotLight {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    glm::vec3 position;
    glm::vec3 direction;
    float cutoff;
};