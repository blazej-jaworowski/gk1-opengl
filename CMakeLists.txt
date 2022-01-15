cmake_minimum_required(VERSION 3.10)

project(garden)

add_executable(garden src/main.cpp
                      src/Window.cpp src/Window.h
                      src/Model.cpp src/Model.h
                      src/Shader.cpp src/Shader.h
                      src/ObjLoader.cpp src/ObjLoader.h
                      src/Garden.cpp src/Garden.h
                      src/structs.h)

set_property(TARGET garden PROPERTY CXX_STANDARD 17)
target_include_directories(garden PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)
target_link_libraries(garden glfw GLEW GL)

file(COPY "models" DESTINATION ".")
file(COPY "shaders" DESTINATION ".")