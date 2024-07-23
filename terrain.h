// terrain.h
// Defines the Terrain class for generating and rendering 3D terrain

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

    // Public methods
    bool loadHeightmap(const std::string& filename);
    void generate();
    void render() const;
    void initComputeShader();
    void computeNormals();
    void setShowNormals(bool);
    int getTriangleCount() const;
    
    bool showNormals;

private:
    // Private member variables
    int gridSize;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<unsigned char> heightmapData;
    int heightmapWidth;
    int heightmapHeight;

    GLuint computeProgram;
    GLuint heightMapTexture;
    GLuint normalMapTexture;

    // Private methods
    float getHeight(int x, int z) const;
    glm::vec3 calculateColor(float height) const;
    void renderNormals() const;
    std::string loadShaderSource(const std::string& filename);
    std::vector<float> normals;  
    void calculateNormals();        
    GLuint vertexBuffer;
    GLuint indexBuffer;
    GLuint normalBuffer;
    void setupBuffers();
};

#endif