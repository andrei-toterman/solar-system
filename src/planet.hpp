#pragma once

#include <GL/glew.h>

#include <vector>
#include "vertex.hpp"
#include "SOIL2.h"

// asta ii planeta de baza. inca nu are chestii care sa orbiteze in juru ei. o sa aiba un vector<Planet> cel mai probabil
struct Planet {
    // toti vertecshii planetei, care au deja in ei tot ce le trebe
    std::vector<Vertex> vertices;
    // indicii vertecshilor. un vertex o sa fie folosit in mai multe triunghiuri, asa ca in loc sa tinem acelasi vertex de mai multe ori, tinem numa indexu lui din vectoru vertices
    std::vector<GLuint> indices;
    // id-u la textura. inca nu are functie de load
    GLuint              texture;
    // vao - Vertex Array Objectu unei planete. in mod normal asa se face treaba. ai unu de asta per obiect si ii dai bind cand vrei sa-l desenezi
    // in el se tin toate chestiile legate de configuratia vertecshilor, si asa nu mai tre setati din nou la fiecare draw call
    // vbo - Vertex Buffer Object. in el e lista de vertecshi, cu tot cu pozitii, normale si coordonate de textura; basically tine informatiile din vectoru vertices
    // ebo - Element Buffer Object. lista de indecshi din vbo pe care sa-i deseneze; basically tine ce e in vectoru indices
    // de exemplu, daca ai un vertex cu pozitia (1, 2, 3), si vrei sa-l desenezi de 3 ori, in VBO ai vertexul (1, 2, 3) doar odata si in EBO ii ai indexu de 3 ori aka 0, 0, 0
    // asta ii util pentru ca vertecshii dintr-un model pot sa faca parte din mai multe triunghiuri din modelu ala
    GLuint              vao{}, vbo{}, ebo{};

    // pozitia
    glm::vec3 position;
    // marimea unei planete, pe x, y si z
    float     scale;

    float distance;

    float rotation_speed;

    std::vector<Planet> satellites;

    Planet(const glm::vec3& _position, float _scale, float _distance, float _rotation_speed,
           const char* texture_path);

    void render() const;
};
