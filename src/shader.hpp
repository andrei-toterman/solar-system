#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>

struct Shader {
    GLuint id;

    Shader(const char* vertex_shader_path, const char* fragment_shader_path);

    static std::string read_shader_source(const char* path);

    static GLuint compile_shader(const char* path, GLenum type);

    void set_mvp(const glm::mat4& mvp) const;
};
