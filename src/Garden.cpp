#include "Garden.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "ObjLoader.h"

void Garden::init(int width, int height) {
    ObjLoader loader("teapot.obj");
    models.emplace_back(loader.vertices, loader.normals, loader.vertex_count,
                        loader.faces, loader.face_count, "vertex_shader.vert",
                        "fragment_shader.frag");

    models[0].set_material({glm::vec3(1.0f, 1.0f, 0.0f),
                            glm::vec3(1.0f, 1.0f, 0.0f),
                            glm::vec3(1.0f, 1.0f, 0.0f), 256.0f});

    set_sun_light({glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.3f, 0.3f, 0.3f),
                   glm::vec3(1.0f, 1.0f, 1.0f),
                   glm::normalize(glm::vec3(0.0f, 1.0f, -1.0f))});

    update_projection(width, height);
    update_camera(glm::vec3(5, 5, 4), glm::vec3(0, 0, 1.0f));
}

void Garden::draw() {
    float angle = (float)glfwGetTime();
    float radius = 7.0f;
    set_sun_light({glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f),
                   glm::vec3(1.0f, 1.0f, 1.0f),
                   glm::normalize(glm::vec3(-1.0f, -1.0f, -3.0f))});
    for (auto &model : models) {
        glm::mat4 model_matrix(1.0f);
        model_matrix =
            glm::rotate(model_matrix, (float)glfwGetTime(), glm::vec3(0, 0, 1));
        model_matrix = glm::rotate(model_matrix, M_PI_2f32, glm::vec3(1, 0, 0));
        model.set_model_matrix(model_matrix);

        model.draw();
    }
}

void Garden::update_camera(glm::vec3 position, glm::vec3 looking_at) {
    for (auto &model : models) {
        model.set_view_matrix(
            glm::lookAt(position, looking_at, glm::vec3(0, 0, 1)));
    }
}

void Garden::update_projection(int width, int height) {
    for (auto &model : models) {
        model.set_projection_matrix(glm::perspective(
            glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f));
    }
}

void Garden::set_sun_light(SunLight sun_light) {
    for (auto &model : models) {
        model.set_sun_light(sun_light);
    }
}