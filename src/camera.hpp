#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <array>
#include "state.hpp"

struct Camera {
    glm::vec3 position;
    glm::vec3 front{};
    glm::vec3 right{};
    glm::vec3 up{};

    float oy_angle{ -90.0f };
    float ox_angle{};

    float movement_speed{ 50.0f };
    float mouse_sensitivity{ 0.3f };
    float fov{ 45.0f };

    explicit Camera(const glm::vec3& _position);

    [[nodiscard]] glm::mat4 view_matrix() const;

    void update(const std::array<bool, State::keys_n>& held_keys, State::Delta& delta);

    void move(float delta_time, const std::array<bool, State::keys_n>& held_keys);

    void look(float delta_x, float delta_y);

    void zoom(float delta_scroll);

    void compute_vectors();
};
