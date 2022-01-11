cmake_minimum_required(VERSION 3.10)

project(garden)


add_executable(garden src/main.cpp glad/src/glad.c
                      src/Window.cpp src/Window.h)

target_include_directories(garden PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)
target_link_libraries(garden glfw dl)