#pragma once

#include <array>
#include <GLFW/glfw3.h>

const unsigned int WIDTH  = 1280;
const unsigned int HEIGHT = 720;

// aici tinem toate chestiile care se schimba de-a lungu programului. is toate la un loc ca sa poata fi pasate usor dintr-un loc in altu
struct State {
    struct Delta {
        float time{ 0.0f };
        float mouse_x{ 0.0f };
        float mouse_y{ 0.0f };
        float mouse_scroll{ 0.0f };
    }      delta;

    float  last_time{ 0.0f };
    double last_mouse_x{ WIDTH / 2.0 };
    double last_mouse_y{ HEIGHT / 2.0 };


    enum Keys {
        W, A, S, D, keys_n
    };

    std::array<bool, keys_n> held_keys{};
};
