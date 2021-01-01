#pragma once

#include "shader.hpp"

#include <glm/glm.hpp>

struct BasicShader : Shader {
    BasicShader(const char* vertex_shader_path, const char* fragment_shader_path);

    void set_mvp(const glm::mat4& mat) const;
};
