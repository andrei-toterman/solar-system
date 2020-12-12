#include "planet.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/gtx/rotate_vector.hpp>

Planet::Planet(const char* texture_path) :
        texture{ SOIL_load_OGL_texture(texture_path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS) } {
    glBindTexture(GL_TEXTURE_2D, texture);
    if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
        GLfloat anisotropy;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // basically codu de la lab da mai frumix
    const unsigned int precision = 48;
    vertices.reserve((precision + 1) * (precision + 1));
    indices.reserve(precision * precision * 6);

    for (int i = 0; i <= precision; i++) {
        float    phi = glm::pi<float>() * (float) i / precision;
        for (int j   = 0; j <= precision; j++) {
            float theta = glm::pi<float>() * 2.0f * (float) j / precision;
            auto  x     = -glm::cos(theta) * glm::sin(phi);
            auto  y     = glm::cos(phi);
            auto  z     = glm::sin(theta) * glm::sin(phi);
            auto  u     = (float) j / precision;
            auto  v     = (float) i / precision;
            vertices.push_back({{ x, y, z },
                                { x, y, z },
                                { u, v }});

            if (i == precision || j == precision) {
                continue;
            }

            indices.push_back(i * (precision + 1) + j);
            indices.push_back(i * (precision + 1) + j + 1);
            indices.push_back((i + 1) * (precision + 1) + j);
            indices.push_back(i * (precision + 1) + j + 1);
            indices.push_back((i + 1) * (precision + 1) + j + 1);
            indices.push_back((i + 1) * (precision + 1) + j);
        }
    }

    // generam VAOu, VBOu si EBOu la planeta
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // setam vaou ca fiind cel curent
    glBindVertexArray(vao);
    // setam si vbou ca fiind cel curent
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // ii zicem la vbo sa-si ia datele din vertices
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // setam ebou ca cel curent si ii zicem sa-si ia din indices indecshii
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // aci ii zicem la VAO cum sunt aranjate datele in VBO, si VAOu o sa tina minte
    // acuma in VBO e o lista lunga de float-uri, care sunt aranjati in grupuri de astea (pos.x, pos.y, pos.z)(norm.x, norm,y, norm.z)(tc.x, tc.y)
    // deci tre sa ii zicem la VAO ca un grup e de marimea unui Vertex, ca pe prima pozitie sunt 3 floaturi, ca pe a doua sunt inca 3 floaturi si ca pe a treia sunt doua floaturi
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position_coords));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal_coords));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texture_coords));

    // dam un-bind la VAO, ca sa nu ramana activ daca nu trebe
    glBindVertexArray(0);
}

Planet::Planet(const char* texture_path, glm::vec3 _position, float _radius, float _rotation_speed,
               glm::vec3 _rotation_axis) : Planet{ texture_path } {
    position       = _position;
    radius         = _radius;
    rotation_speed = _rotation_speed;
    rotation_axis  = _rotation_axis;
}

Planet::Planet(Planet& _parent, const char* texture_path, float _orbit_radius, float _radius, float _rotation_speed,
               glm::vec3 _rotation_axis, float _orbit_speed, glm::vec3 _orbit_axis) : Planet{ texture_path } {
    parent         = &_parent;
    position       = { _orbit_radius, 0.0f, 0.0f };
    radius         = _radius;
    rotation_speed = _rotation_speed;
    rotation_axis  = _rotation_axis;
    orbit_speed    = _orbit_speed;
    orbit_axis     = _orbit_axis;
}

glm::vec3 Planet::absolute_position(float base_orbit_radius) {
    if (parent) {
        return glm::rotate(position * base_orbit_radius, orbit_angle, orbit_axis) +
               parent->absolute_position(base_orbit_radius);
    }
    return position;
}

void Planet::update(float time) {
    rotation_angle = rotation_speed * time;
    if (parent) {
        orbit_angle = orbit_speed * time;
    }
}

void Planet::render() const {
    // dam bind la textura din obiectu asta
    glBindTexture(GL_TEXTURE_2D, texture);
    // dam bind la VAO, si el stie deja de unde sa isi ia vertecshii si cum sunt aranjati, ca o tinut minte care ii VBOu, si si EBOu
    glBindVertexArray(vao);
    // desenam vertecshii cu indicii din EBO
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    // dam unbind la VAO
    glBindVertexArray(0);
    // dam unbind la textura
    glBindTexture(GL_TEXTURE_2D, 0);
}
