#pragma once

#include <glm/glm.hpp>

// un obiect o sa aiba mai multi vertecshi, si fiecare vertex o sa isi aiba coordonatele pentru pozitie, normala si textura
struct Vertex {
    glm::vec3 position_coords{};
    glm::vec3 normal_coords{};
    glm::vec2 texture_coords{};
};
