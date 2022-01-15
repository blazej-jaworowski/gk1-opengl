#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Model.h"
#include "structs.h"

class Garden {
  private:
    std::vector<Model> models;
    glm::vec3 get_sky_color(float time);
    glm::vec3 get_sun_color(float time);
    void update_sun(float time);
    void update_sky(float time);

  public:
    void init(int width, int height);
    void set_reflection_model(bool blinn);
    void add_scene_models();
    Model &add_model_from_files(std::string obj_filename,
                                std::string vertex_filename,
                                std::string fragment_filename);
    void update_camera(glm::vec3 pos, glm::vec3 looking_at);
    void update_projection(int width, int height);
    void set_dir_light(DirLight sun_light, int index);
    void set_dir_light(float r, float g, float b, glm::vec3 direction,
                       int index);
    void update();
    void draw();
};