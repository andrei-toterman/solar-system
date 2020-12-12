#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "planet.hpp"
#include "shader.hpp"
#include "state.hpp"
#include "camera.hpp"

void mouse_callback(GLFWwindow* window, double x, double y);

void scroll_callback(GLFWwindow* window, double, double delta_scroll);

void key_callback(GLFWwindow* window, int key, int, int action, int);

int main() {
    if (!glfwInit()) {
        std::cerr << "failed to initialize GLFW" << std::endl;
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Solar System", nullptr, nullptr);
    if (!window) {
        std::cerr << "failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "failed to initialize GLEW" << std::endl;
        return 1;
    }

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    State  state{};
    Camera camera{{ 0.0f, 0.0f, 0.0f }};
    glfwSetWindowUserPointer(window, &state);

    const glm::vec3 UP{ 0.0f, 1.0f, 0.0f };

    // orbit radius: 10^7 km
    // radius: 10^5 km
    // rotation speed: 10^3 km/h
    // orbit speed: 10 km/s
    Planet sun{ "res/sun.jpg", { 0.0f, 0.0f, 0.0f }, 69.5f * 0.2f, 1.99f, UP };
    Planet mercury{ sun, "res/mercury.jpg", 5.7f, 0.24f, 0.01f, UP, 4.74f };
    Planet venus{ sun, "res/venus.jpg", 10.8f, 0.6f, 0.006f, UP, 3.5f };
    Planet earth{ sun, "res/earth.jpg", 14.9f, 0.63f, 1.67f, UP, 2.98f };
    Planet moon{ earth, "res/moon.jpg", 0.38f, 0.17f, 1.0f, UP, 0.1f };
    Planet mars{ sun, "res/mars.jpg", 22.7f, 0.33f, 0.86f, UP, 2.41f };
    Planet jupiter{ sun, "res/jupiter.jpg", 77.8f, 6.9f, 45.58f, UP, 1.31f };
    Planet saturn{ sun, "res/saturn.jpg", 143.4f, 5.82f, 36.84f, UP, 0.97f };
    Planet uranus{ sun, "res/uranus.jpg", 287.1f, 2.53f, 14.79f, UP, 0.68f };
    Planet neptune{ sun, "res/neptune.jpg", 449.5, 2.46f, 9.71f, UP, 0.54f };

    std::array objects{ &sun, &mercury, &venus, &earth, &moon, &mars, &jupiter, &saturn, &uranus, &neptune };

    Shader shader{ "shaders/vertex.glsl", "shaders/fragment.glsl" };
    glUseProgram(shader.id);

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        auto time = (float) glfwGetTime();
        state.delta.time = time - state.last_time;
        state.last_time  = time;

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.update(state.held_keys, state.delta);
        glm::mat4 proj{ glm::perspective(glm::radians(camera.fov), (float) WIDTH / HEIGHT, 0.1f, 10000.0f) };
        glm::mat4 view{ camera.view_matrix() };

        for (const auto object : objects) {
            object->update(state.last_time * state.base_speed);
        }
        for (const auto object : objects) {
            glm::mat4 model{ 1.0f };
            model = glm::translate(model, object->absolute_position(state.base_orbit_radius));
            model = glm::rotate(model, object->rotation_angle, object->rotation_axis);
            model = glm::scale(model, glm::vec3{ object->radius * state.base_radius });
            shader.set_mvp(proj * view * model);
            object->render();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow* window, double x, double y) {
    auto state = (State*) glfwGetWindowUserPointer(window);

    state->delta.mouse_x = (float) (x - state->last_mouse_x);
    state->delta.mouse_y = (float) (state->last_mouse_y - y);

    state->last_mouse_x = x;
    state->last_mouse_y = y;
}

void scroll_callback(GLFWwindow* window, double, double delta_scroll) {
    auto state = (State*) glfwGetWindowUserPointer(window);
    state->delta.mouse_scroll = (float) delta_scroll;
}

void key_callback(GLFWwindow* window, int key, int, int action, int) {
    auto state = (State*) glfwGetWindowUserPointer(window);

    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, true);
                break;
            case GLFW_KEY_W: state->held_keys[State::W] = true;
                break;
            case GLFW_KEY_A: state->held_keys[State::A] = true;
                break;
            case GLFW_KEY_S: state->held_keys[State::S] = true;
                break;
            case GLFW_KEY_D: state->held_keys[State::D] = true;
                break;
            default: break;
        }
    }
    if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_W: state->held_keys[State::W] = false;
                break;
            case GLFW_KEY_A: state->held_keys[State::A] = false;
                break;
            case GLFW_KEY_S: state->held_keys[State::S] = false;
                break;
            case GLFW_KEY_D: state->held_keys[State::D] = false;
                break;
            default: break;
        }
    }
}
