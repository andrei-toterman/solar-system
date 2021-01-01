#include "camera.hpp"
#include "state.hpp"

Camera::Camera(const glm::vec3& _position) : position{ _position } {
    compute_vectors();
}

glm::mat4 Camera::view_matrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::update(State& state) {
    move(state.delta.time, state.camera_movement);
    look(state.delta.mouse_x, state.delta.mouse_y);
    zoom(state.delta.mouse_scroll);
    state.delta.mouse_x      = 0.0f;
    state.delta.mouse_y      = 0.0f;
    state.delta.mouse_scroll = 0.0f;
}

void Camera::move(float delta_time, const std::array<bool, State::movement_n>& movement) {
    float velocity = (movement[State::SLOW] ? slow_movement_speed : movement_speed) * delta_time;
    position += (float) movement[State::FORWARD] * velocity * front;
    position -= (float) movement[State::LEFT] * velocity * right;
    position -= (float) movement[State::BACKWARD] * velocity * front;
    position += (float) movement[State::RIGHT] * velocity * right;
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
