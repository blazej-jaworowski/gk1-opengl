#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>

class Window {
  private:
    int width, height;
    GLFWwindow *window;
    void loop();

  public:
    Window(int width, int height);
    void terminate();
    void run();
    void update_size_values(int width, int height);
};