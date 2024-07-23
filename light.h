// light.h
// Defines the Light structure and declares the renderCube function

#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

// Structure to represent a light source
struct Light {
  glm::vec3 position;
  glm::vec3 color;
  float intensity;

  Light(glm::vec3 pos, glm::vec3 col = glm::vec3(1.0f), float intens = 1.0f)
      : position(pos), color(col), intensity(intens) {}
};

// Function to render a cube (used for visualizing light sources)
void renderCube();

#endif // LIGHT_H