#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <GL/glew.h>
#include <iostream>

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
    GLuint VAO, VBO, EBO;

    void setupBuffers();
};

#endif