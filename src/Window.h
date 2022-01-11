#pragma once

#include <glad/glad.h>

#include <GLFW/glfw3.h>

class Window {
  private:
    int width, height;
    GLFWwindow *window;
    static void framebuffer_size_callback(GLFWwindow *window, int width,
                                          int height);
    void loop();

  public:
    Window(int width, int height);
    void terminate();
    void run();
};