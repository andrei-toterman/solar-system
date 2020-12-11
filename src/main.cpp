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

float base_size     = 0.2f;
float base_distance = 6.0f;

void render_moons(const Planet& planet, Shader shader, glm::mat4 proj, glm::mat4 view, glm::mat4 model) {
    for (const auto& moon : planet.satellites) {
        model = glm::rotate(model, moon.rotation_speed * (float) glfwGetTime() / (moon.distance * base_distance),
                            { 0.0f, 1.0f, 0.0f });
        model = glm::translate(model, { moon.distance * base_distance, 0.0f, moon.distance * base_distance });
        model = glm::scale(model, glm::vec3{ moon.scale } * base_size);
        shader.set_mvp(proj * view * model);
        moon.render();
    }
}

void render_planets(const Planet& sun, Shader shader, glm::mat4 proj, glm::mat4 view) {
    for (const auto& planet : sun.satellites) {
        glm::mat4 model{ 1.0f };
        model = glm::translate(model, planet.position);
        model = glm::rotate(model, planet.rotation_speed * (float) glfwGetTime() / (planet.distance * base_distance),
                            { 0.0f, 1.0f, 0.0f });
        model = glm::translate(model, { planet.distance * base_distance, 0.0f, planet.distance * base_distance });
        model = glm::scale(model, glm::vec3{ planet.scale } * base_size);
        shader.set_mvp(proj * view * model);
        planet.render();
        render_moons(planet, shader, proj, view, model);
    }
}

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

    // aci setam ce callbackuri se apeleaza cand se misca mouseu, scrollu sau se apasa pe taste
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    // asta face cursoru sa dispara si sa ramana in fereastra. dai exit sa iesi
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    State  state{};
    Camera camera{{ 0.0f, 0.0f, 0.0f }};
    // ii dam la window app-stateu, ca sa poata fi accesat in callbackuri
    glfwSetWindowUserPointer(window, &state);

    if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
        GLfloat anisoSetting = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
    }

    Planet sun{{ 0.0f, 0.0f, 0.0f }, 8.0f, 0.0f, 1, "res/sun.jpg" };
    Planet mercury{{ 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 1, "res/mercury.jpg" };
    Planet venus{{ 0.0f, 0.0f, 0.0f }, 1.15f, 1.09f, 1, "res/venus.jpg" };
    Planet earth{{ 0.0f, 0.0f, 0.0f }, 1.16f, 1.16f, 1, "res/earth.jpg" };
    Planet moon{{ 0.0f, 0.0f, 0.0f }, 1.0f, 0.155f, 1, "res/moon.jpg" };
    Planet mars{{ 0.0f, 0.0f, 0.0f }, 1.04f, 1.30f, 1, "res/mars.png" };
    Planet jupiter{{ 0.0f, 0.0f, 0.0f }, 3.81f, 2.27f, 1, "res/jupiter.jpg" };
    Planet saturn{{ 0.0f, 0.0f, 0.0f }, 3.33f, 3.41f, 1, "res/saturn.jpg" };
    Planet uranus{{ 0.0f, 0.0f, 0.0f }, 1.95f, 5.95f, 1, "res/uranus.jpg" };
    Planet neptune{{ 0.0f, 0.f, 0.0f }, 1.93f, 8.79f, 1, "res/neptune.jpg" };
    Planet pluto{{ 0.0f, 0.0f, 0.0f }, 0.95f, 11.25f, 1, "res/pluto.jpg" };

    earth.satellites.push_back(std::move(moon));
    sun.satellites.push_back(std::move(mercury));
    sun.satellites.push_back(std::move(venus));
    sun.satellites.push_back(std::move(earth));
    sun.satellites.push_back(std::move(mars));
    sun.satellites.push_back(std::move(jupiter));
    sun.satellites.push_back(std::move(saturn));
    sun.satellites.push_back(std::move(uranus));
    sun.satellites.push_back(std::move(neptune));
    sun.satellites.push_back(std::move(pluto));

    // unicu shader. poate o sa mai avem si altele
    Shader shader{ "shaders/vertex.glsl", "shaders/fragment.glsl" };
    glUseProgram(shader.id);

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        // asta calculeaza cat timp se scurge intre cadre, ca sa poti sa te folosesti de asta cand misti camera si sa fie mai smooth
        auto time = (float) glfwGetTime();
        state.delta.time = time - state.last_time;
        state.last_time  = time;

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // updatam camera in functie de ce chestii face useru
        camera.update(state.held_keys, state.delta);

        glm::mat4 proj{ glm::perspective(glm::radians(camera.fov), (float) WIDTH / HEIGHT, 0.1f, 100.0f) };
        glm::mat4 view{ camera.view_matrix() };

        // iteram prin obiecte si le calculam la fiecare model matrixu in functie de pozitia si marimea lor, le trimitem la shader, si le dam draw
        glm::mat4 model{ 1.0f };
        model = glm::translate(model, sun.position);
        model = glm::scale(model, glm::vec3{ sun.scale });
        shader.set_mvp(proj * view * model);
        sun.render();
        render_planets(sun, shader, proj, view);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow* window, double x, double y) {
    auto state = (State*) glfwGetWindowUserPointer(window);

    // astea trebe pentru ca GLFW iti da pozitia curenta a mouseului in fereastra, da pe camera o intereseaza cat si cum s-o miscat mouseu fata de unde era data trecuta, nu fix unde e acuma
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
            // apesti ESC, se opreste
            case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, true);
                break;
                // setam care keys is apasate
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
    // setam care keys nu mai is apasate
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
