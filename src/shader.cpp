#include "shader.hpp"

Shader::Shader(const char* vertex_shader_path, const char* fragment_shader_path) {
    id = glCreateProgram();

    GLuint vertex_shader_id   = compile_shader(vertex_shader_path, GL_VERTEX_SHADER);
    GLuint fragment_shader_id = compile_shader(fragment_shader_path, GL_FRAGMENT_SHADER);

    glAttachShader(id, vertex_shader_id);
    glAttachShader(id, fragment_shader_id);
    glLinkProgram(id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
}

std::string Shader::read_shader_source(const char* path) {
    std::ifstream input_file(path, std::ios::in | std::ios::ate);
    std::string   source_code;
    source_code.reserve(input_file.tellg());
    input_file.seekg(0, std::ios::beg);
    source_code.assign(std::istreambuf_iterator<char>{ input_file }, {});
    return source_code;
}

GLuint Shader::compile_shader(const char* path, GLenum type) {
    std::string source_code{ read_shader_source(path) };
    GLuint      shader_id = glCreateShader(type);

    const char* code_string = source_code.data();
    int code_size = source_code.size();
    glShaderSource(shader_id, 1, &code_string, &code_size);
    glCompileShader(shader_id);
    return shader_id;
}

void Shader::set_mvp(const glm::mat4& mvp) const {
    glUniformMatrix4fv(glGetUniformLocation(id, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
}
