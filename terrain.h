// terrain.h
// Defines the Terrain class for generating and rendering 3D terrain

#ifndef TERRAIN_H
#define TERRAIN_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

class Terrain {
public:
  // Constructor and destructor
  Terrain(int gridSize);
  ~Terrain();

  // Public methods
  bool loadHeightmap(const std::string &filename);
  void generate();
  void render() const;
  void initComputeShader();
  void computeNormals();
  void setShowNormals(bool);
  int getTriangleCount() const;
  void setUseCPUOnly(bool useCPU) { useCPUOnly = useCPU; }

  // Public member variable
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

  std::vector<float> normals;
  GLuint vertexBuffer;
  GLuint indexBuffer;
  GLuint normalBuffer;

  bool useCPUOnly;

  // Private methods
  float getHeight(int x, int z) const;
  glm::vec3 calculateColor(float height) const;
  void renderNormals() const;
  std::string loadShaderSource(const std::string &filename);
  void calculateNormals();
  void calculateNormalsCPU();
  void setupBuffers();
};

#endif // TERRAIN_H