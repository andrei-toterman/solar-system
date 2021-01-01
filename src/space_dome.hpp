#pragma once

#include <GL/glew.h>
#include <vector>

#include "render_object.hpp"

struct SpaceDome : RenderObject {
    explicit SpaceDome(const char* texture_path);

    [[nodiscard]] glm::mat4 model_matrix(const State& state) const override;

    void update(const State& state) override;

    void render() const override;

    glm::vec3 position{};
};
