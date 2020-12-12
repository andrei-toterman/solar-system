#pragma once

#include <GL/glew.h>

#include <vector>
#include "vertex.hpp"
#include "SOIL2.h"

struct Planet {
    // cu asta construim chestiile care nu orbiteaza in juru la nimic aka soarele
    Planet(const char* texture_path, glm::vec3 _position, float _radius, float _rotation_speed,
           glm::vec3 _rotation_axis);

    // cu asta construim planetele care orbiteaza in juru la ceva
    Planet(Planet& _parent, const char* texture_path, float _orbit_radius, float _radius, float _rotation_speed,
           glm::vec3 _rotation_axis, float _orbit_speed, glm::vec3 _orbit_axis = { 0.0f, 1.0f, 0.0f });

    // fiecare obiect trebuie sa stie sa-si calculeze pozitia absoluta in funcite de parinte
    glm::vec3 absolute_position(float base_orbit_radius);

    void update(float time);

    void render() const;

    // marimea unei planete, pe x, y si z
    float     radius{};
    // cat s-o rotit in juru propriei axe
    float     rotation_angle{};
    // axa de rotatie in jurul propriei axe
    glm::vec3 rotation_axis{};
private:
    explicit Planet(const char* texture_path);

    // planeta in jurul careia orbiteaza
    Planet* parent{};
    // toti vertecshii planetei, care au deja in ei tot ce le trebe
    std::vector<Vertex> vertices;
    // indicii vertecshilor. un vertex o sa fie folosit in mai multe triunghiuri, asa ca in loc sa tinem acelasi vertex de mai multe ori, tinem numa indexu lui din vectoru vertices
    std::vector<GLuint> indices;
    // id-u la textura. inca nu are functie de load
    GLuint              texture{};
    // vao - Vertex Array Objectu unei planete. in mod normal asa se face treaba. ai unu de asta per obiect si ii dai bind cand vrei sa-l desenezi
    // in el se tin toate chestiile legate de configuratia vertecshilor, si asa nu mai tre setati din nou la fiecare draw call
    // vbo - Vertex Buffer Object. in el e lista de vertecshi, cu tot cu pozitii, normale si coordonate de textura; basically tine informatiile din vectoru vertices
    // ebo - Element Buffer Object. lista de indecshi din vbo pe care sa-i deseneze; basically tine ce e in vectoru indices
    // de exemplu, daca ai un vertex cu pozitia (1, 2, 3), si vrei sa-l desenezi de 3 ori, in VBO ai vertexul (1, 2, 3) doar odata si in EBO ii ai indexu de 3 ori aka 0, 0, 0
    // asta ii util pentru ca vertecshii dintr-un model pot sa faca parte din mai multe triunghiuri din modelu ala
    GLuint              vao{}, vbo{}, ebo{};
    // pozitia relativ la parinte
    glm::vec3           position{};
    // viteza de rotatie in jurul propriei axe
    float               rotation_speed{};
    // viteza de rotatie in jurul parintelui
    float               orbit_speed{};
    // axa de rotatie in jurul parintelui
    glm::vec3           orbit_axis{};
    // cat s-o rotit pe orbita
    float               orbit_angle{};
};
