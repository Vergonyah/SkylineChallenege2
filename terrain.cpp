#include "terrain.h"
#include <cmath>
#include <glm/gtc/noise.hpp>


Terrain::Terrain(int gridSize) : gridSize(gridSize) {}

Terrain::~Terrain() {}

void Terrain::generate() {
    float size = 50.0f;  
    float step = size / static_cast<float>(gridSize - 1);

    for (int z = 0; z < gridSize; ++z) {
        for (int x = 0; x < gridSize; ++x) {
            float xPos = x * step - size / 2.0f;
            float zPos = z * step - size / 2.0f;
            float yPos = getHeight(xPos, zPos);
            vertices.push_back(xPos);
            vertices.push_back(yPos);
            vertices.push_back(zPos);
        }
    }

    for (int z = 0; z < gridSize - 1; ++z) {
        for (int x = 0; x < gridSize - 1; ++x) {
            unsigned int topLeft = z * gridSize + x;
            unsigned int topRight = topLeft + 1;
            unsigned int bottomLeft = (z + 1) * gridSize + x;
            unsigned int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
}

float Terrain::getHeight(float x, float z) const {
    float scale = 0.1f;
    return glm::simplex(glm::vec2(x, z) * scale) * 5.0f;
}

glm::vec3 Terrain::calculateColor(float height) const {
    if (height < -2.0f) return glm::vec3(0.2f, 0.2f, 0.8f);  
    if (height < 0.0f)  return glm::vec3(0.4f, 0.4f, 0.8f);  
    if (height < 2.0f)  return glm::vec3(0.8f, 0.7f, 0.4f);  
    if (height < 4.0f)  return glm::vec3(0.4f, 0.8f, 0.4f);  
    return glm::vec3(0.5f, 0.5f, 0.5f);  
}

void Terrain::render() const {
    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < indices.size(); i += 3) {
        for (int j = 0; j < 3; ++j) {
            int index = indices[i + j];
            float x = vertices[index * 3];
            float y = vertices[index * 3 + 1];
            float z = vertices[index * 3 + 2];
            
            glm::vec3 color = calculateColor(y);
            glColor3f(color.r, color.g, color.b);
            glVertex3f(x, y, z);
        }
    }
    glEnd();
}
