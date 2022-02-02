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

    if (glewInit() != GLEW_OK) {
        std::cerr << "ERROR: Failed to initialize GLEW\n";
        exit(-1);
    }

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(
        window, [](GLFWwindow *window, int width, int height) {
            glViewport(0, 0, width, height);
            Window *w =
                reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
            w->update_size_values(width, height);
        });
    glfwSetKeyCallback(window, Window::key_callback);

    loop();
}

void Window::loop() {
    garden.init(width, height);
    float prev_time = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time = glfwGetTime();

        garden.update(time - prev_time);
        garden.draw();

        prev_time = time;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Window::update_size_values(int width, int height) {
    this->width = width;
    this->height = height;
    garden.update_projection(width, height);
}

void Window::key_callback(GLFWwindow *window, int key, int scancode, int action,
                          int mods) {
    Window *w = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));

    if (action != GLFW_PRESS)
        return;

    // float step = 0.2f;
    // glm::vec3 translation{};
    switch (key) {
    // case GLFW_KEY_W:
    //     translation.x -= step;
    //     break;
    // case GLFW_KEY_S:
    //     translation.x += step;
    //     break;
    // case GLFW_KEY_A:
    //     translation.y -= step;
    //     break;
    // case GLFW_KEY_D:
    //     translation.y += step;
    //     break;
    // case GLFW_KEY_Q:
    //     translation.z -= step;
    //     break;
    // case GLFW_KEY_E:
    //     translation.z += step;
    //     break;
    case GLFW_KEY_1:
        w->garden.set_flower_destination(0);
        break;
    case GLFW_KEY_2:
        w->garden.set_flower_destination(1);
        break;
    case GLFW_KEY_N:
        w->garden.toggle_day();
        break;
    case GLFW_KEY_B:
        w->garden.toggle_reflection_model();
        break;
    case GLFW_KEY_8:
        w->garden.change_camera(0);
        break;
    case GLFW_KEY_9:
        w->garden.change_camera(1);
        break;
    case GLFW_KEY_0:
        w->garden.change_camera(2);
        break;
    }

    // translation += w->garden.get_bee_destination();
    // w->garden.set_bee_destination(translation);

    // std::cout << translation.x << ' ' << translation.y << ' ' <<
    // translation.z
    //           << '\n';
}