#pragma once

#include <array>
#include <GLFW/glfw3.h>

const unsigned int WIDTH  = 1280;
const unsigned int HEIGHT = 720;

struct State {
    struct Delta {
        float time{ 0.0f };
        float mouse_x{ 0.0f };
        float mouse_y{ 0.0f };
        float mouse_scroll{ 0.0f };
    } delta;

    float  last_time{ 0.0f };
    double last_mouse_x{ WIDTH / 2.0 };
    double last_mouse_y{ HEIGHT / 2.0 };

    enum {
        FORWARD, LEFT, BACKWARD, RIGHT, SLOW, movement_n
    };

    std::array<bool, movement_n> camera_movement{};

    float base_radius       = 1.0f;
    float base_orbit_radius = 3.0f;
    float base_speed        = 1.0f;

    bool mouse_pressed{ false };
    bool mouse_in_settings{ false };
};
