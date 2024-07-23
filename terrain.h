#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

class Terrain {
public:
    Terrain(int gridSize);
    ~Terrain();

    bool loadHeightmap(const std::string& filename);
    void generate();
    void render() const;
    void initComputeShader();
    void computeNormals();
    void setShowNormals(bool);

    bool showNormals;

private:
    int gridSize;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<unsigned char> heightmapData;
    int heightmapWidth;
    int heightmapHeight;

    GLuint computeProgram;
    GLuint heightMapTexture;
    GLuint normalMapTexture;

    float getHeight(int x, int z) const;
    glm::vec3 calculateColor(float height) const;
    void renderNormals() const;
    std::string loadShaderSource(const std::string& filename);
};

#endif