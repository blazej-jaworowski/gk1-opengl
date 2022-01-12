#include "Window.h"

#include <iostream>

#include "Model.h"

void Window::framebuffer_size_callback(GLFWwindow *window, int width,
                                       int height) {
    glViewport(0, 0, width, height);
}

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
    glfwSetFramebufferSizeCallback(window, Window::framebuffer_size_callback);

    loop();
}

void Window::loop() {
    float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                        0.0f,  0.0f,  0.5f, 0.0f};
    int faces[] = {0, 1, 2};
    Model model1(vertices, 3, faces, 1, "vertex_shader.vert",
                 "fragment_shader.frag");

    vertices[1] = 0.5f;
    Model model2(vertices, 3, faces, 1, "vertex_shader.vert",
                 "fragment_shader.frag");
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        model1.draw();
        model2.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}