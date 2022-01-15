#include "Garden.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "ObjLoader.h"

Model &Garden::add_model_from_files(std::string obj_filename,
                                    std::string vertex_filename,
                                    std::string fragment_filename) {
    ObjLoader loader(obj_filename);
    return models.emplace_back(
        loader.vertices, loader.normals, loader.vertex_count, loader.faces,
        loader.face_count, vertex_filename, fragment_filename);
}

void Garden::add_scene_models() {
    Model &model =
        add_model_from_files("models/bee.obj", "shaders/vertex_shader.vert",
                             "shaders/fragment_shader.frag");
    model.set_material({glm::vec3(0.913, 0.737, 0.1841),
                        glm::vec3(0.913, 0.737, 0.1841),
                        glm::vec3(0.0f, 0.0f, 0.0f), 1.0f});
    model.rotate(M_PI_2f32, glm::vec3(1.0f, 0.0f, 0.0f));
    model.translate(glm::vec3(0, 0, 3));

    model =
        add_model_from_files("models/plane.obj", "shaders/vertex_shader.vert",
                             "shaders/fragment_shader.frag");
    model.set_material({glm::vec3(0, 0.858, 0.023), glm::vec3(0, 0.858, 0.023),
                        glm::vec3(0.0f, 0.0f, 0.0f), 1.0f});

    model =
        add_model_from_files("models/rose.obj", "shaders/vertex_shader.vert",
                             "shaders/fragment_shader.frag");
    model.set_material(
        {glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), 32.0f});
    model.rotate(0.9, glm::vec3(0, 1, 0));
    model.translate(glm::vec3(1, 1, 2));

    model =
        add_model_from_files("models/rose2.obj", "shaders/vertex_shader.vert",
                             "shaders/fragment_shader.frag");
    model.set_material(
        {glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), 32.0f});
    model.rotate(M_PI_2, glm::vec3(1, 0, 0));
    model.scale(0.2f);
    model.translate(glm::vec3(-1.2, -1.2, 0));
}

void Garden::init(int width, int height) {
    add_scene_models();
    update_projection(width, height);

    set_reflection_model(false);
    update();
}

void Garden::update() {
    float time = (float)glfwGetTime();

    set_reflection_model((int)time % 2);

    float radius = 17.0f;
    // update_camera(
    //     glm::vec3(radius * std::sin(time), radius * std::cos(time), 5),
    //     glm::vec3(0, 0, 2));
    update_camera(glm::vec3(5, -17, 7), glm::vec3(0, 0, 2));

    update_sun(time);
    update_sky(time);
}

void Garden::update_sun(float time) {
    glm::vec3 sunset_color(1, 0.4, 0);
    glm::vec3 noon_color(0.8, 0.8, 0.4);
    float factor = std::sin(time * M_2_PI);
    if (factor > 0) {
        glm::vec3 sun_color =
            factor * (sunset_color + factor * (noon_color - sunset_color));
        set_dir_light(sun_color.x, sun_color.y, sun_color.z,
                      glm::normalize(glm::vec3(std::cos(time * M_2_PI), -0.1f,
                                               -std::sin(time * M_2_PI))),
                      0);
    } else {
        set_dir_light(0, 0, 0, glm::vec3(0, 0, -1), 0);
    }
    set_dir_light(0.08, 0.08, 0.08, glm::vec3(0, 0, -1), 1);
}

void Garden::update_sky(float time) {
    glClearColor(0, 0, 0, 1);
    glm::vec3 sunset_color(0, 0, 1);
    glm::vec3 noon_color(0, 0.5, 1);
    float factor = std::sin(time * M_2_PI);
    if (factor > -0.2) {
        glm::vec3 sky_color =
            factor * (noon_color + factor * (noon_color - sunset_color));
        glClearColor(sky_color.x, sky_color.y, sky_color.z, 1.0f);
    } else {
        glClearColor(0, 0, 0, 1);
    }
}

void Garden::draw() {
    for (auto &model : models) {
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

void Garden::set_dir_light(DirLight dir_light, int index) {
    for (auto &model : models) {
        model.set_dir_light(dir_light, index);
    }
}

void Garden::set_dir_light(float r, float g, float b, glm::vec3 direction,
                           int index) {
    DirLight dir_light{glm::vec3(0.4f * r, 0.4f * g, 0.4f * b),
                       glm::vec3(r, g, b), glm::vec3(r, g, b),
                       glm::normalize(direction)};
    set_dir_light(dir_light, index);
}

void Garden::set_reflection_model(bool blinn) {
    for (auto &model : models) {
        model.set_reflection_model(blinn);
    }
}