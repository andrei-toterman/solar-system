#pragma once

#include <GL/glew.h>

#include <vector>

#include "vertex.hpp"

// basically ca o planeta, da e un cub, si n-are textura. e numa pentru test, ca sa te prinzi daca ai orientat bine chestiile, ca o sfera e rotunda si nu te prinzi
struct Cube {
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    unsigned int              vao{}, vbo{}, ebo{};

    glm::vec3 scale;
    glm::vec3 position;

    Cube(const glm::vec3& _position, const glm::vec3& _scale);

    void render() const;
};
