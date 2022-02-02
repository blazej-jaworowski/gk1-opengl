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
    // Bee always first
    Model &model_bee =
        add_model_from_files("models/bee.obj", "shaders/vertex_shader.vert",
                             "shaders/fragment_shader.frag");
    model_bee.set_material({glm::vec3(0.913, 0.737, 0.1841),
                            glm::vec3(0.913, 0.737, 0.1841),
                            glm::vec3(0.0f, 0.0f, 0.0f), 1.0f});
    model_bee.scale(0.2);

    // Destination sphere always second
    Model &model_sphere =
        add_model_from_files("models/sphere.obj", "shaders/vertex_shader.vert",
                             "shaders/fragment_shader.frag");
    model_sphere.set_material(
        {glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 32.0f});
    model_sphere.scale(0.02);

    Model &model_plane =
        add_model_from_files("models/plane.obj", "shaders/vertex_shader.vert",
                             "shaders/fragment_shader.frag");
    model_plane.set_material({glm::vec3(0, 0.858, 0.023),
                              glm::vec3(0, 0.858, 0.023),
                              glm::vec3(0.0f, 0.0f, 0.0f), 1.0f});

    Model &model_rose1 =
        add_model_from_files("models/rose.obj", "shaders/vertex_shader.vert",
                             "shaders/fragment_shader.frag");
    model_rose1.set_material(
        {glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), 32.0f});
    model_rose1.rotate(0.9, glm::vec3(0, 1, 0));
    model_rose1.translate(glm::vec3(2, 2, 2));
    destinations.push_back(glm::vec3(2, 3.2, 3.8));

    Model &model_rose2 =
        add_model_from_files("models/rose2.obj", "shaders/vertex_shader.vert",
                             "shaders/fragment_shader.frag");
    model_rose2.set_material(
        {glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), 32.0f});
    model_rose2.rotate(M_PI_2, glm::vec3(1, 0, 0));
    model_rose2.scale(0.2f);
    model_rose2.translate(glm::vec3(-2, -2, 0));
    destinations.push_back(glm::vec3(-2, -2.2, 3.2));
}

void Garden::init(int width, int height) {
    add_scene_models();
    bee = Bee(&models.at(0), &models.at(1));
    update_projection(width, height);

    set_reflection_model(false);
    update_sun(0.25f);
    update_sky(0.25f);
    update_camera(glm::vec3(5, -17, 7), glm::vec3(0, 0, 2));
}

void Garden::update(float dt) {
    bee.update(dt);
    update_camera(glm::vec3(20, 0, 7), bee.get_position());
}

void Garden::update_sun(float time) {
    glm::vec3 sunset_color(1, 0.4, 0);
    glm::vec3 noon_color(0.8, 0.8, 0.4);
    float factor = std::sin(time * 2 * M_PI);
    if (factor > 0) {
        glm::vec3 sun_color =
            factor * (sunset_color + factor * (noon_color - sunset_color));
        set_dir_light(sun_color.x, sun_color.y, sun_color.z,
                      glm::normalize(glm::vec3(std::cos(time * 2 * M_PI), -0.1f,
                                               -std::sin(time * 2 * M_PI))),
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
    float factor = std::sin(time * 2 * M_PI);
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

void Garden::set_bee_destination(glm::vec3 dest) { bee.set_destination(dest); }

void Garden::set_flower_destination(int index) {
    set_bee_destination(destinations.at(index));
}

glm::vec3 Garden::get_bee_destination() { return bee.get_destination(); }