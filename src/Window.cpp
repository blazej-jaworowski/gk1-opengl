#include "Window.h"

#include <iostream>

#include "Garden.h"
#include "Model.h"
#include "ObjLoader.h"

#include <glm/gtc/matrix_transform.hpp>

Window::Window(int width, int height) : width(width), height(height) {}

void Window::terminate() { glfwTerminate(); }

void Window::run() {
    if (!glfwInit()) {
        std::cerr << "ERROR: Failed to initialize GLFW\n";
        exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "Garden", NULL, NULL);
    glfwSetWindowUserPointer(window, this);

    if (window == NULL) {
        std::cerr << "ERROR: Failed to create window\n";
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "ERROR: Failed to initialize GLAD\n";
        exit(-1);
    }

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(
        window, [](GLFWwindow *window, int width, int height) {
            glViewport(0, 0, width, height);
            Window *w = (Window *)glfwGetWindowUserPointer(window);
            w->update_size_values(width, height);
        });

    loop();
}

void Window::loop() {
    garden.init(width, height);
    glClearColor(0, 0, 0, 1);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        garden.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Window::update_size_values(int width, int height) {
    this->width = width;
    this->height = height;
    garden.update_projection(width, height);
}