#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>

// asta ii un shader intreg compilat si linkuit. are si fragment si vertex shader
struct Shader {
    GLuint id;

    Shader(const char* vertex_shader_path, const char* fragment_shader_path);

    static std::string read_shader_source(const char* path);

    static GLuint compile_shader(const char* path, GLenum type);

    // cu functii de astea o sa setam variabilele uniforme. acuma setam numa matricea aia Model View Projection
    // cand mai bagam uniforme in shadere, mai adaugam aici functii de astea
    void set_mvp(const glm::mat4& mvp) const;
};
