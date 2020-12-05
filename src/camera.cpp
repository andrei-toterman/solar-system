#include "camera.hpp"
#include "state.hpp"

Camera::Camera(const glm::vec3& _position) : position{ _position } {
    compute_vectors();
}

glm::mat4 Camera::view_matrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::update(std::array<bool, State::keys_n> held_keys, State::Delta& delta) {
    move(delta.time, held_keys);
    look(delta.mouse_x, delta.mouse_y);
    zoom(delta.mouse_scroll);
    // glfw nu seteaza offseturile la 0 daca nu se intapmpla nimic, asa ce le setam aci dupa ce le folosim
    delta.mouse_x      = 0.0f;
    delta.mouse_y      = 0.0f;
    delta.mouse_scroll = 0.0f;
}

void Camera::move(float delta_time, std::array<bool, State::keys_n> held_keys) {
    float velocity = movement_speed * delta_time;
    // held_keys are boolene, deci daca e false se inmulteste cu 0, deci nu se intampla nimic, daca e true, se inmulteste cu 1, deci se aduna in mod normal; e mai scurt decat cu cate un if la fiecare
    position += held_keys[State::W] * velocity * front;
    position -= held_keys[State::A] * velocity * right;
    position -= held_keys[State::S] * velocity * front;
    position += held_keys[State::D] * velocity * right;
}

void Camera::look(float delta_x, float delta_y) {
    oy_angle += delta_x * mouse_sensitivity;
    ox_angle = glm::clamp(ox_angle + delta_y * mouse_sensitivity, -89.0f, 89.0f);
    compute_vectors();
}

void Camera::zoom(float delta_scroll) {
    fov = glm::clamp(fov - delta_scroll, 1.0f, 45.0f);
}

void Camera::compute_vectors() {
    front = glm::normalize(glm::vec3{
            cos(glm::radians(oy_angle)) * cos(glm::radians(ox_angle)),
            sin(glm::radians(ox_angle)),
            sin(glm::radians(oy_angle)) * cos(glm::radians(ox_angle))
    });
    right = glm::normalize(glm::cross(front, { 0.0f, 1.0f, 0.0f }));
    up    = glm::normalize(glm::cross(right, front));
}
