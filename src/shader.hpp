#pragma once

#include <GL/glew.h>
#include <string>

struct Shader {
    GLuint id;

    Shader(const char* vertex_shader_path, const char* fragment_shader_path);

    static std::string read_shader_source(const char* path);

    static GLuint compile_shader(const char* path, GLenum type);
};
