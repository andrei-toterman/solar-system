#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <array>
#include "state.hpp"

struct Camera {
    glm::vec3 position;
    // vectorii astia iti dau directia relativ fata de camera
    glm::vec3 front{};
    glm::vec3 right{};
    glm::vec3 up{};

    // cu unghiurile asta te rotesti in juru axelor ox si oy relativ la scena
    float oy_angle{ -90.0f };
    float ox_angle{};

    // viteza si zoom
    float movement_speed{ 50.0f };
    float mouse_sensitivity{ 0.3f };
    float fov{ 45.0f };

    explicit Camera(const glm::vec3& _position);

    // calculeaza matricea de View in functie de vectorii camerei
    [[nodiscard]] glm::mat4 view_matrix() const;

    // aici udatam camera, aka apelam functiile de mai jos
    void update(const std::array<bool, State::keys_n>& held_keys, State::Delta& delta);

    // schimba pozitia camerei cu WASD. da nu si directia in care se uita. numa pozitia
    void move(float delta_time, const std::array<bool, State::keys_n>& held_keys);

    // cu asta misti camera cu mouseu aka schimbi unghiurile alea cu ox si oy
    void look(float delta_x, float delta_y);

    // dai zoom cu scrollu
    void zoom(float delta_scroll);

    // calculeaza vectorii in functie de unghiurile alea
    void compute_vectors();
};
