#include "Window.h"

#include <iostream>

#include "Model.h"
#include "ObjLoader.h"

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
    ObjLoader loader("teapot.obj");
    Model model((float *)&loader.vertices.at(0), loader.vertices.size(),
                (uint32_t *)&loader.faces.at(0), loader.faces.size(),
                "vertex_shader.vert", "fragment_shader.frag");

    model.set_model_matrix(glm::mat4(1.0f));
    model.set_projection_matrix(glm::perspective(
        glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f));
    model.set_view_matrix(glm::lookAt(glm::vec3(5, 5, 4), glm::vec3(0, 0, 1),
                                      glm::vec3(0, 0, 1)));

    glClearColor(0, 0, 0, 1);
    while (!glfwWindowShouldClose(window)) {
        glm::mat4 model_matrix(1.0f);
        model_matrix =
            glm::rotate(model_matrix, (float)glfwGetTime(), glm::vec3(0, 0, 1));
        model_matrix = glm::rotate(model_matrix, M_PI_2f32, glm::vec3(1, 0, 0));
        model.set_model_matrix(model_matrix);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        model.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}