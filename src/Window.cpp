#include "Window.h"

#include <iostream>

#include "Model.h"

#include <glm/gtc/matrix_transform.hpp>

void Window::framebuffer_size_callback(GLFWwindow *window, int width,
                                       int height) {
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
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
    float vertices[] = {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1};
    uint32_t faces[] = {1, 2, 3, 0, 2, 3, 0, 1, 3, 0, 1, 2};
    Model model1(vertices, 4, faces, 4, "vertex_shader.vert",
                 "fragment_shader.frag");
    Model model2(vertices, 4, faces, 4, "vertex_shader.vert",
                 "fragment_shader.frag");

    model1.set_model_matrix(glm::mat4(1.0f));
    model1.set_projection_matrix(glm::perspective(
        glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f));
    model1.set_view_matrix(glm::lookAt(
        glm::vec3(2, 2, 2), glm::vec3(0, 0, 0.5f), glm::vec3(0, 0, 1)));

    model2.set_model_matrix(glm::mat4(1.0f));
    model2.set_projection_matrix(glm::perspective(
        glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f));
    model2.set_view_matrix(glm::lookAt(
        glm::vec3(2, 2, 2), glm::vec3(0, 0, 0.5f), glm::vec3(0, 0, 1)));

    glClearColor(0, 0, 0, 1);
    while (!glfwWindowShouldClose(window)) {
        model1.set_model_matrix(glm::rotate(
            glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0, 0, 1)));

        model2.set_model_matrix(glm::rotate(
            glm::mat4(1.0f), (float)glfwGetTime() * 2, glm::vec3(0, 1, 1)));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        model1.draw();
        model2.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}