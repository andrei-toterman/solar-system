#include "basic_shader.hpp"

#include <glm/gtc/type_ptr.hpp>

BasicShader::BasicShader(const char* vertex_shader_path, const char* fragment_shader_path) :
        Shader{ vertex_shader_path, fragment_shader_path } {
}

void BasicShader::set_mvp(const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(id, "mvp"), 1, GL_FALSE, glm::value_ptr(mat));
}
