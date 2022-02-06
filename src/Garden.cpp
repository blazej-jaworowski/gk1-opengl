#include "Garden.h"

#include <iostream>

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
    model_plane.set_material({glm::vec3(0, 0.658, 0.113),
                              glm::vec3(0, 0.658, 0.113),
                              glm::vec3(0.0f, 0.0f, 0.0f), 1.0f});

    Model &model_rose1 =
        add_model_from_files("models/rose.obj", "shaders/vertex_shader.vert",
                             "shaders/fragment_shader.frag");
    model_rose1.set_material({glm::vec3(0.8, 0, 0), glm::vec3(0.8, 0, 0),
                              glm::vec3(0.8, 0, 0), 32.0f});
    model_rose1.rotate(0.9, glm::vec3(0, 1, 0));
    model_rose1.translate(glm::vec3(2, 2, 2));
    destinations.push_back(glm::vec3(2, 3.2, 3.8));

    Model &model_rose2 =
        add_model_from_files("models/rose2.obj", "shaders/vertex_shader.vert",
                             "shaders/fragment_shader.frag");
    model_rose2.set_material({glm::vec3(0, 0, 0.8), glm::vec3(0, 0, 0.8),
                              glm::vec3(0, 0, 0.8), 32.0f});
    model_rose2.rotate(M_PI_2, glm::vec3(1, 0, 0));
    model_rose2.scale(0.2f);
    model_rose2.translate(glm::vec3(-2, -2, 0));
    destinations.push_back(glm::vec3(-2, -2.2, 3.2));

    Model &model_poppy =
        add_model_from_files("models/poppy.obj", "shaders/vertex_shader.vert",
                             "shaders/fragment_shader.frag");
    model_poppy.set_material({glm::vec3(0.8, 0.8, 0), glm::vec3(0.8, 0.8, 0),
                              glm::vec3(0.8, 0.8, 0), 32.0f});
    model_poppy.rotate(M_PI_2, glm::vec3(1, 0, 0));
    model_poppy.translate({0.5, -2.5, -0.5});
    destinations.push_back(glm::vec3(0.6, -2.4, 1.8));

    Model &model_rock =
        add_model_from_files("models/sphere.obj", "shaders/vertex_shader.vert",
                             "shaders/fragment_shader.frag");
    model_rock.set_material({glm::vec3(0.6, 0.6, 1), glm::vec3(0.6, 0.6, 1),
                             glm::vec3(0.6, 0.6, 1), 32.0f});
    model_rock.translate(glm::vec3(1, 0, -0.5f));
    model_rock.scale(0.5);
}

void Garden::init(int width, int height) {
    add_scene_models();
    bee = Bee(&models.at(0), &models.at(1));
    update_projection(width, height);

    set_reflection_model(reflection_model);
    set_fog_color(glm::vec3(0.7f));
    update_sun(0.25f);
    update_sky(0.25f);

    change_camera(0);
    set_flower_destination(0);
    set_spot_light({{0, 0, 0},
                    {1, 1, 1},
                    {1, 1, 1},
                    1,
                    0.22,
                    0.20,
                    {0.6, -2.4, 1.5},
                    {0, 0, 1},
                    -1},
                   1);
}

void Garden::update(float dt) {
    bee.update(dt);
    update_time(dt);
    update_camera();
    update_spotlight();
    update_fog(dt);
}

void Garden::update_spotlight() {
    glm::vec3 position = bee.get_position();
    glm::vec3 direction = glm::normalize(bee.get_destination() - position);
    set_spot_light({{0, 0, 0},
                    {1, 1, 1},
                    {1, 1, 1},
                    1,
                    0.14,
                    0.07,
                    position,
                    direction,
                    0.95f},
                   0);
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
        sky_color =
            factor * (noon_color + factor * (noon_color - sunset_color));
        glClearColor(sky_color.x, sky_color.y, sky_color.z, 1.0f);
    } else {
        glClearColor(0, 0, 0, 1);
        sky_color = glm::vec3(0);
    }
}

void Garden::draw() {
    for (auto &model : models) {
        model.draw();
    }
}

void Garden::set_camera(glm::vec3 position, glm::vec3 looking_at) {
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

void Garden::toggle_day() {
    time_until_target_day_time = 2;
    target_day_time = day ? 0.51f : 0.25f;
    day = !day;
}

void Garden::update_time(float dt) {
    if (time_until_target_day_time < 0) {
        return;
    }
    day_time += (target_day_time - day_time) * dt / time_until_target_day_time;
    if (day_time)
        time_until_target_day_time -= dt;

    set_fog_color(glm::vec3((0.51f - day_time) * 0.7f / 0.26f));
    update_sky(day_time);
    update_sun(day_time);
}

void Garden::change_camera(int index) {
    if (index == 0) {
        set_camera({-10, 10, 7}, {0, 0, 3});
        // set_camera({0, 0.1, 7}, {0, 0, 3});
    }
    camera = index;
}

void Garden::update_camera() {
    switch (camera) {
    case 1:
        set_camera({10, 10, 7}, bee.get_position());
        break;
    case 2:
        glm::vec3 offset = glm::vec3(0, 0, 0);
        glm::vec3 bee_dir = glm::normalize(bee.get_velocity());
        glm::vec3 camera_pos = bee.get_position() - 2.0f * bee_dir + offset;
        if (camera_pos.z < 0)
            camera_pos.z = 0;
        glm::vec3 camera_dir = camera_pos + bee_dir + offset;
        set_camera(camera_pos, camera_dir);
        break;
    }
}

void Garden::toggle_reflection_model() {
    reflection_model = !reflection_model;
    set_reflection_model(reflection_model);
}

void Garden::set_spot_light(SpotLight spot_light, int index) {
    for (Model &model : models) {
        model.set_spot_light(spot_light, index);
    }
}

void Garden::set_fog_dist(float fog_dist) {
    for (Model &model : models) {
        model.set_fog_dist(fog_dist);
    }
}

void Garden::set_fog_enabled(bool fog_enabled) {
    for (Model &model : models) {
        model.set_fog_enabled(fog_enabled);
    }
}

void Garden::toggle_fog() {
    fog_enabled = !fog_enabled;
    if (fog_enabled) {
        set_fog_enabled(true);
    }
    time_until_fog = 2;
}

void Garden::update_fog(float dt) {
    float sky_factor =
        fog_enabled ? 1 - time_until_fog / 2 : time_until_fog / 2;
    sky_factor = std::min(1.0f, sky_factor * 8);
    glm::vec3 foggy_sky_color = glm::mix(sky_color, fog_color, sky_factor);
    glClearColor(foggy_sky_color.x, foggy_sky_color.y, foggy_sky_color.z, 1);

    float target = fog_enabled ? 20 : 1000;
    if (time_until_fog <= 0) {
        fog_dist = target;
        set_fog_dist(fog_dist);
        if (!fog_enabled) {
            set_fog_enabled(false);
        }
        return;
    }

    float k = dt / time_until_fog;
    k = fog_enabled ? std::sqrt(k) : k * k;
    fog_dist += (target - fog_dist) * k;
    set_fog_dist(fog_dist);
    time_until_fog -= dt;
}

void Garden::set_fog_color(glm::vec3 fog_color) {
    this->fog_color = fog_color;
    for (Model &model : models) {
        model.set_fog_color(fog_color);
    }
}