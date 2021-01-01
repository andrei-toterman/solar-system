#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shader.hpp"

struct PhongShader : Shader {
    PhongShader(const char* vertex_shader_path, const char* fragment_shader_path);

    void set_mvp(const glm::mat4& mat) const;

    void set_mv(const glm::mat4& mat) const;

    void set_normal_matrix(const glm::mat4& mat) const;

    void set_light_position(const glm::vec3& vec) const;

    void set_light_ambient(const glm::vec4& vec) const;

    void set_light_diffuse(const glm::vec4& vec) const;

    void set_light_specular(const glm::vec4& vec) const;

    void set_global_ambient(const glm::vec4& vec) const;

    void set_shininess(float value) const;
};
