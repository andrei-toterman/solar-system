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

    // vector de planete. in viitor cel mai probabil aici o sa initializam doar gen soarele sau ce o fi, la care ii adaugam dupa planetele
    std::vector<Planet> objects;
    objects.emplace_back(glm::vec3{ -2.0f, 0.0f, -15.0f }, glm::vec3{ 1.0f });
    objects.emplace_back(glm::vec3{ 2.0f, 0.0f, -15.0f }, glm::vec3{ 1.0f });
    objects.emplace_back(glm::vec3{ 0.0f, 2.0f, -15.0f }, glm::vec3{ 1.0f, 3.0f, 1.0f });

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
        for (const auto& object: objects) {
            glm::mat4 model{ 1.0f };
            model = glm::translate(model, object.position);
            model = glm::scale(model, object.scale);
            shader.set_mvp(proj * view * model);
            object.render();
        }

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
