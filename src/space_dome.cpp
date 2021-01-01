#include "space_dome.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"

SpaceDome::SpaceDome(const char* texture_path) : RenderObject{ texture_path } {
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

void SpaceDome::render() const {
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(vao);
    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

glm::mat4 SpaceDome::model_matrix(const State& state) const {
    glm::mat4 model{ 1.0f };
    model = glm::translate(model, position);
    return model;
}

void SpaceDome::update(const State& state) {
    position = state.camera.position;
}
