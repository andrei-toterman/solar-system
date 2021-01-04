#include "ring.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

Ring::Ring(Planet& _parent, const char* texture_path, float fill, float _radius, glm::vec3 _rotation_axis,
           float _rotation_speed) :
        RenderObject{ texture_path },
        parent{ _parent },
        radius{ _radius },
        rotation_axis{ _rotation_axis },
        rotation_speed{ _rotation_speed } {
    const unsigned int precision = 100;
    vertices.reserve((precision + 1) * (precision + 1));
    indices.reserve(precision * precision * 6);

    for (int i = 0; i <= precision; i++) {
        auto     phi = glm::pi<float>() * (float) i / precision;
        for (int j   = 0; j <= precision; j++) {
            auto theta = glm::pi<float>() * 2.0f * (float) j / precision;
            auto x     = (fill * glm::cos(theta) + 1.0f) * glm::cos(2.0f * phi);
            auto y     = fill * glm::sin(theta) * 0.1f;
            auto z     = (fill * glm::cos(theta) + 1.0f) * glm::sin(2.0f * phi);
            auto u     = (float) j / precision;
            auto v     = (float) i / precision;
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

glm::mat4 Ring::model_matrix(const State& state) const {
    glm::mat4 model{ 1.0f };
    model = glm::translate(model, parent.absolute_position(state.base_orbit_radius));
    model = glm::scale(model, glm::vec3{ radius * state.base_radius });
    model = glm::rotate(model, rotation_angle, rotation_axis);
    glm::vec3 axis_align{ glm::cross({ 0.0f, 1.0f, 0.0f }, glm::normalize(rotation_axis)) };
    model = glm::rotate(model, glm::asin(glm::length(axis_align)), axis_align);
    return model;
}

void Ring::update(const State& state) {
    rotation_angle += state.base_speed * rotation_speed * state.delta.time;
}

void Ring::render() const {
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
