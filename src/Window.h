#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Garden.h"

class Window {
  private:
    int width, height;

    GLFWwindow *window;
    void loop();

    static void key_callback(GLFWwindow *window, int key, int scancode,
                             int action, int mods);

  public:
    Garden garden;

    Window(int width, int height);
    void terminate();
    void run();
    void update_size_values(int width, int height);
};