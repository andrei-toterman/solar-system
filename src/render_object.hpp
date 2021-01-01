#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "vertex.hpp"
#include "state.hpp"

struct RenderObject {
    explicit RenderObject(const char* texture_path);

    void create_array_object();

    [[nodiscard]] virtual glm::mat4 model_matrix(const State& state) const = 0;

    virtual void update(const State& state) = 0;

    virtual void render() const = 0;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GLuint              texture{};
    GLuint              vao{}, vbo{}, ebo{};
};
