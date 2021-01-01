#include "phong_shader.hpp"

#include <glm/gtc/type_ptr.hpp>

PhongShader::PhongShader(const char* vertex_shader_path, const char* fragment_shader_path) :
        Shader{ vertex_shader_path, fragment_shader_path } {
}

void PhongShader::set_mvp(const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(id, "mvp"), 1, GL_FALSE, glm::value_ptr(mat));
}

void PhongShader::set_mv(const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(id, "mv"), 1, GL_FALSE, glm::value_ptr(mat));
}

void PhongShader::set_normal_matrix(const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(id, "normal_matrix"), 1, GL_FALSE, glm::value_ptr(mat));
}

void PhongShader::set_light_position(const glm::vec3& vec) const {
    glUniform3fv(glGetUniformLocation(id, "light_position"), 1, glm::value_ptr(vec));
}

void PhongShader::set_light_ambient(const glm::vec4& vec) const {
    glUniform4fv(glGetUniformLocation(id, "light_ambient"), 1, glm::value_ptr(vec));
}

void PhongShader::set_light_diffuse(const glm::vec4& vec) const {
    glUniform4fv(glGetUniformLocation(id, "light_diffuse"), 1, glm::value_ptr(vec));
}

void PhongShader::set_light_specular(const glm::vec4& vec) const {
    glUniform4fv(glGetUniformLocation(id, "light_specular"), 1, glm::value_ptr(vec));
}

void PhongShader::set_global_ambient(const glm::vec4& vec) const {
    glUniform4fv(glGetUniformLocation(id, "global_ambient"), 1, glm::value_ptr(vec));
}

void PhongShader::set_shininess(float value) const {
    glUniform1f(glGetUniformLocation(id, "shininess"), value);
}
