#include "cube.hpp"

Cube::Cube(const glm::vec3& _position, const glm::vec3& _scale) : position{ _position }, scale{ _scale } {

    vertices.push_back({{ -1.0f, -1.0f, -1.0f }});
    vertices.push_back({{ 1.0f, -1.0f, -1.0f }});
    vertices.push_back({{ 1.0f, -1.0f, 1.0f }});
    vertices.push_back({{ -1.0f, -1.0f, 1.0f }});
    vertices.push_back({{ -1.0f, 1.0f, -1.0f }});
    vertices.push_back({{ 1.0f, 1.0f, -1.0f }});
    vertices.push_back({{ 1.0f, 1.0f, 1.0f }});
    vertices.push_back({{ -1.0f, 1.0f, 1.0f }});

    indices.assign({
                           0, 1, 2, //
                           0, 2, 3, //
                           2, 3, 6, //
                           3, 6, 7, //
                           1, 2, 5, //
                           2, 5, 6, //
                           0, 1, 4, //
                           1, 4, 5, //
                           0, 4, 7, //
                           0, 3, 7, //
                           4, 5, 7, //
                           5, 6, 7, //
                   });

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position_coords));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal_coords));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texture_coords));

    glBindVertexArray(0);
}

void Cube::render() const {
    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}
