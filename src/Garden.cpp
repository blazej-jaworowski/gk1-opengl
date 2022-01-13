#include "Garden.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "ObjLoader.h"

void Garden::init(int width, int height) {
    ObjLoader loader("teapot.obj");
    Model model(loader.vertices, loader.normals, loader.vertex_count,
                loader.faces, loader.face_count, "vertex_shader.vert",
                "fragment_shader.frag");

    model.set_color(1, 0, 1);

    models.push_back(model);

    update_projection(width, height);
    update_camera(glm::vec3(5, 5, 5), glm::vec3(0, 0, 1));
}

void Garden::draw() {
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