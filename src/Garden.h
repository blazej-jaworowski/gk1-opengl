#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Bee.h"
#include "Model.h"
#include "structs.h"

class Garden {
  private:
    int width, height;

    std::vector<Model> models;
    Bee bee;
    std::vector<glm::vec3> destinations;

    bool day = true;
    float day_time = 0.25f;
    float target_day_time;
    float time_until_target_day_time = -1.0f;

    int camera = 0;

    bool reflection_model = false;

    float time_until_fog = 0;
    float fog_dist = 1000;
    bool fog_enabled = false;

    glm::vec3 sky_color;
    glm::vec3 fog_color;

    void update_sun(float time);
    void update_sky(float time);
    void update_time(float dt);
    void set_camera(glm::vec3 pos, glm::vec3 looking_at);
    void update_camera();
    void set_reflection_model(bool blinn);
    void set_fog_dist(float fog_dist);
    void set_fog_enabled(bool fog_enabled);
    void set_fog_color(glm::vec3 fog_color);
    void update_fog(float dt);
    void init_uniforms(int width, int height);

  public:
    void init(int width, int height);
    void toggle_reflection_model();
    void add_scene_models();
    Model &add_model_from_files(std::string obj_filename,
                                std::string vertex_filename,
                                std::string fragment_filename);
    void update_projection(int width, int height);
    void set_dir_light(DirLight sun_light, int index);
    void set_dir_light(float r, float g, float b, glm::vec3 direction,
                       int index);
    void set_spot_light(SpotLight spot_light, int index);
    void update(float dt);
    void draw();
    void set_bee_destination(glm::vec3 dest);
    void set_flower_destination(int index);
    glm::vec3 get_bee_destination();
    void toggle_day();
    void change_camera(int index);
    void update_spotlight();
    void toggle_fog();
    void change_shaders(std::string vertex, std::string fragment);
};