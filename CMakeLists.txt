cmake_minimum_required(VERSION 3.17)
project(solar_system)

set(CMAKE_CXX_STANDARD 17)

include_directories(imgui)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/basic_shader.cpp
        src/camera.cpp
        src/phong_shader.cpp
        src/planet.cpp
        src/render_object.cpp
        src/ring.cpp
        src/shader.cpp
        src/space_dome.cpp
        imgui/imgui.cpp
        imgui/imgui_impl_glfw.cpp
        imgui/imgui_impl_opengl3.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_widgets.cpp
        )

cmake_policy(SET CMP0072 NEW)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw GLEW::glew OpenGL::GL soil2)
