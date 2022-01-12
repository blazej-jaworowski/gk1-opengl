cmake_minimum_required(VERSION 3.10)

project(garden)

add_executable(garden src/main.cpp glad/src/glad.c
                      src/Window.cpp src/Window.h
                      src/Model.cpp src/Model.h
                      src/Shader.cpp src/Shader.h)

target_include_directories(garden PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)
target_link_libraries(garden glfw dl)

configure_file(src/vertex_shader.vert vertex_shader.vert COPYONLY)
configure_file(src/fragment_shader.frag fragment_shader.frag COPYONLY)