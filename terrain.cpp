#include "terrain.h"
#include <cmath>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Terrain::Terrain(int gridSize) : gridSize(gridSize), heightmapWidth(0), heightmapHeight(0) {}

Terrain::~Terrain() {}

bool Terrain::loadHeightmap(const std::string& filename) {
    int channels;
    unsigned char* data = stbi_load(filename.c_str(), &heightmapWidth, &heightmapHeight, &channels, 1);
    if (!data) {
        std::cerr << "Failed to load heightmap: " << filename << std::endl;
        return false;
    }

    heightmapData = std::vector<unsigned char>(data, data + heightmapWidth * heightmapHeight);
    stbi_image_free(data);
    return true;
}

void Terrain::generate() {
    if (heightmapData.empty()) {
        std::cerr << "No heightmap data loaded. Call loadHeightmap() first." << std::endl;
        return;
    }

    float size = 50.0f;
    float step = size / static_cast<float>(gridSize - 1);

    vertices.clear();
    indices.clear();

    for (int z = 0; z < gridSize; ++z) {
        for (int x = 0; x < gridSize; ++x) {
            float xPos = x * step - size / 2.0f;
            float zPos = z * step - size / 2.0f;
            float yPos = getHeight(x, z);
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

float Terrain::getHeight(int x, int z) const {
    if (x < 0 || x >= gridSize || z < 0 || z >= gridSize) {
        return 0.0f;
    }
    
    int heightmapX = static_cast<int>((static_cast<float>(x) / gridSize) * heightmapWidth);
    int heightmapZ = static_cast<int>((static_cast<float>(z) / gridSize) * heightmapHeight);
    
    int index = heightmapZ * heightmapWidth + heightmapX;
    return static_cast<float>(heightmapData[index]) / 255.0f * 10.0f;  
}

glm::vec3 Terrain::calculateColor(float height) const {
    if (height < 1.0f) return glm::vec3(0.2f, 0.2f, 0.8f);  
    if (height < 3.0f) return glm::vec3(0.8f, 0.7f, 0.4f);  
    if (height < 6.0f) return glm::vec3(0.4f, 0.8f, 0.4f);  
    if (height < 8.0f) return glm::vec3(0.5f, 0.5f, 0.5f);  
    return glm::vec3(1.0f, 1.0f, 1.0f);  
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