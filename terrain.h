#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>


class Terrain {
public:
    Terrain(int gridSize);
    ~Terrain();

    void generate();
    void render() const;

private:
    int gridSize;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    float getHeight(float x, float z) const;
    glm::vec3 calculateColor(float height) const;
};

#endif 