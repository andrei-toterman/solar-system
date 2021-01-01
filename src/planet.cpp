#include "planet.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "state.hpp"

Planet::Planet(const char* texture_path) : RenderObject{ texture_path } {
    const unsigned int precision = 48;
    vertices.reserve((precision + 1) * (precision + 1));
    indices.reserve(precision * precision * 6);

    for (int i = 0; i <= precision; i++) {
        float    phi = glm::pi<float>() * (float) i / precision;
        for (int j   = 0; j <= precision; j++) {
            float theta = glm::pi<float>() * 2.0f * (float) j / precision;
            auto  x     = -glm::cos(theta) * glm::sin(phi);
            auto  y     = glm::cos(phi);
            auto  z     = glm::sin(theta) * glm::sin(phi);
            auto  u     = (float) j / precision;
            auto  v     = (float) i / precision;
            vertices.push_back({{ x, y, z },
                                { x, y, z },
                                { u, v }});

            if (i == precision || j == precision) {
                continue;
            }

            indices.push_back(i * (precision + 1) + j);
            indices.push_back(i * (precision + 1) + j + 1);
            indices.push_back((i + 1) * (precision + 1) + j);
            indices.push_back(i * (precision + 1) + j + 1);
            indices.push_back((i + 1) * (precision + 1) + j + 1);
            indices.push_back((i + 1) * (precision + 1) + j);
        }
    }
    create_array_object();
}

Planet::Planet(const char* texture_path, glm::vec3 _position, float _radius, float _rotation_speed,
               glm::vec3 _rotation_axis) : Planet{ texture_path } {
    position       = _position;
    radius         = _radius;
    rotation_speed = _rotation_speed;
    rotation_axis  = _rotation_axis;
}

Planet::Planet(Planet& _parent, const char* texture_path, float _orbit_radius, float _radius, float _rotation_speed,
               glm::vec3 _rotation_axis, float _orbit_speed, glm::vec3 _orbit_axis) : Planet{ texture_path } {
    parent         = &_parent;
    position       = { _orbit_radius, 0.0f, 0.0f };
    radius         = _radius;
    rotation_speed = _rotation_speed;
    rotation_axis  = _rotation_axis;
    orbit_speed    = _orbit_speed;
    orbit_axis     = _orbit_axis;
}

glm::vec3 Planet::absolute_position(float base_orbit_radius) const {
    if (parent) {
        return glm::rotate(position * base_orbit_radius, orbit_angle, orbit_axis) +
               parent->absolute_position(base_orbit_radius);
    }
    return position;
}

glm::mat4 Planet::model_matrix(const State& state) const {
    glm::mat4 model{ 1.0f };
    model = glm::translate(model, absolute_position(state.base_orbit_radius));
    model = glm::rotate(model, rotation_angle, rotation_axis);
    model = glm::scale(model, glm::vec3{ radius * state.base_radius });
    return model;
}

void Planet::update(const State& state) {
    rotation_angle += state.base_speed * rotation_speed * state.delta.time;
    if (parent) {
        orbit_angle += state.base_speed * orbit_speed * state.delta.time;
    }
}

void Planet::render() const {
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
