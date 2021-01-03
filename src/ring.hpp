#pragma once

#include "render_object.hpp"
#include "planet.hpp"

struct Ring : RenderObject {
    Ring(Planet& _parent, const char* texture_path, float fill, float _radius, glm::vec3 _rotation_axis,
         float _rotation_speed);

    [[nodiscard]] glm::mat4 model_matrix(const State& state) const override;

    void update(const State& state) override;

    void render() const override;

    Planet& parent;
    float radius{};

    // rotation around own axis
    float     rotation_angle{};
    glm::vec3 rotation_axis{};
    float     rotation_speed{};
};
