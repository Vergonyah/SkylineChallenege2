// light.h
#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

struct Light {
    glm::vec3 position;
    glm::vec3 color;
    float intensity;

    Light(glm::vec3 pos, glm::vec3 col = glm::vec3(1.0f), float intens = 1.0f)
        : position(pos), color(col), intensity(intens) {}
};

#endif // LIGHT_H