// terrain.cpp
// Implements the Terrain class methods for generating and rendering 3D terrain

#include "terrain.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Constructor
Terrain::Terrain(int gridSize)
    : showNormals(false), gridSize(gridSize), heightmapWidth(0),
      heightmapHeight(0), computeProgram(0), heightMapTexture(0),
      normalMapTexture(0), useCPUOnly(false) {}

// Destructor
Terrain::~Terrain() {
  // Clean up OpenGL resources
  glDeleteProgram(computeProgram);
  glDeleteTextures(1, &heightMapTexture);
  glDeleteTextures(1, &normalMapTexture);
  glDeleteBuffers(1, &vertexBuffer);
  glDeleteBuffers(1, &indexBuffer);
  glDeleteBuffers(1, &normalBuffer);
}

// Set whether to show normal vectors
void Terrain::setShowNormals(bool show) { showNormals = show; }

// Get the number of triangles in the terrain
int Terrain::getTriangleCount() const { return indices.size() / 3; }

// Load heightmap from an image file
bool Terrain::loadHeightmap(const std::string &filename) {
  int channels;
  unsigned char *data = stbi_load(filename.c_str(), &heightmapWidth,
                                  &heightmapHeight, &channels, 1);
  if (!data) {
    std::cerr << "Failed to load heightmap: " << filename << std::endl;
    return false;
  }

  heightmapData =
      std::vector<unsigned char>(data, data + heightmapWidth * heightmapHeight);
  stbi_image_free(data);
  return true;
}

// Generate terrain mesh from heightmap data
void Terrain::generate() {
  if (heightmapData.empty()) {
    std::cerr << "No heightmap data loaded. Call loadHeightmap() first."
              << std::endl;
    return;
  }

  float size = 50.0f;
  float step = size / static_cast<float>(gridSize - 1);

  vertices.clear();
  indices.clear();

  // Generate vertices
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

  // Generate indices for triangles
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
  calculateNormals();
  setupBuffers();
}

// Set up OpenGL buffers for vertices, indices, and normals
void Terrain::setupBuffers() {
  // Set up vertex buffer
  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  // Set up index buffer
  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);

  // Set up normal buffer
  glGenBuffers(1, &normalBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), nullptr,
               GL_DYNAMIC_DRAW);
}

// Calculate normal vectors for the terrain (CPU version)
void Terrain::calculateNormals() {
  normals.clear();
  normals.resize(vertices.size(), 0.0f);

  // Calculate normals for each triangle
  for (size_t i = 0; i < indices.size(); i += 3) {
    unsigned int i0 = indices[i] * 3;
    unsigned int i1 = indices[i + 1] * 3;
    unsigned int i2 = indices[i + 2] * 3;

    glm::vec3 v1(vertices[i0], vertices[i0 + 1], vertices[i0 + 2]);
    glm::vec3 v2(vertices[i1], vertices[i1 + 1], vertices[i1 + 2]);
    glm::vec3 v3(vertices[i2], vertices[i2 + 1], vertices[i2 + 2]);

    glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

    // Accumulate normals for each vertex
    for (int j = 0; j < 3; ++j) {
      normals[i0 + j] += normal[j];
      normals[i1 + j] += normal[j];
      normals[i2 + j] += normal[j];
    }
  }

  // Normalize the accumulated normals
  for (size_t i = 0; i < normals.size(); i += 3) {
    glm::vec3 n(normals[i], normals[i + 1], normals[i + 2]);
    n = glm::normalize(n);
    normals[i] = n.x;
    normals[i + 1] = n.y;
    normals[i + 2] = n.z;
  }
}

// Get height value from heightmap data
float Terrain::getHeight(int x, int z) const {
  if (x < 0 || x >= gridSize || z < 0 || z >= gridSize) {
    return 0.0f;
  }

  // Map grid coordinates to heightmap coordinates
  int heightmapX =
      static_cast<int>((static_cast<float>(x) / gridSize) * heightmapWidth);
  int heightmapZ =
      static_cast<int>((static_cast<float>(z) / gridSize) * heightmapHeight);

  int index = heightmapZ * heightmapWidth + heightmapX;
  return static_cast<float>(heightmapData[index]) / 255.0f *
         10.0f; // Scale height
}

// Calculate color based on terrain height
glm::vec3 Terrain::calculateColor(float height) const {
  // Simple color gradient based on height
  if (height < 1.0f)
    return glm::vec3(0.2f, 0.2f, 0.8f); // Water
  if (height < 3.0f)
    return glm::vec3(0.8f, 0.7f, 0.4f); // Sand
  if (height < 6.0f)
    return glm::vec3(0.4f, 0.8f, 0.4f); // Grass
  if (height < 8.0f)
    return glm::vec3(0.5f, 0.5f, 0.5f); // Rock
  return glm::vec3(1.0f, 1.0f, 1.0f);   // Snow
}

// Render the terrain
void Terrain::render() const {
  glEnable(GL_LIGHTING);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  // Bind vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glVertexPointer(3, GL_FLOAT, 0, nullptr);

  // Bind normal buffer
  glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
  glNormalPointer(GL_FLOAT, 0, nullptr);

  // Generate and bind color buffer
  std::vector<glm::vec3> colors(vertices.size() / 3);
  for (size_t i = 0; i < vertices.size(); i += 3) {
    colors[i / 3] = calculateColor(vertices[i + 1]);
  }
  GLuint colorBuffer;
  glGenBuffers(1, &colorBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3),
               colors.data(), GL_STATIC_DRAW);
  glColorPointer(3, GL_FLOAT, 0, nullptr);

  // Bind index buffer and draw
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

  // Clean up
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);

  glDeleteBuffers(1, &colorBuffer);

  glDisable(GL_LIGHTING);

  // Render normals if enabled
  if (showNormals) {
    renderNormals();
  }
}

// Initialize compute shader for GPU-based normal calculation
void Terrain::initComputeShader() {
  // Create and compile compute shader
  GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
  std::string computeShaderSource = loadShaderSource("compute_shader.glsl");
  const char *source = computeShaderSource.c_str();
  glShaderSource(computeShader, 1, &source, nullptr);
  glCompileShader(computeShader);

  // Create and link compute program
  computeProgram = glCreateProgram();
  glAttachShader(computeProgram, computeShader);
  glLinkProgram(computeProgram);

  glDeleteShader(computeShader);

  // Create height map texture
  glGenTextures(1, &heightMapTexture);
  glBindTexture(GL_TEXTURE_2D, heightMapTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, gridSize, gridSize, 0, GL_RED,
               GL_FLOAT, vertices.data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Create normal map texture
  glGenTextures(1, &normalMapTexture);
  glBindTexture(GL_TEXTURE_2D, normalMapTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, gridSize, gridSize, 0, GL_RGBA,
               GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

// Compute normals using either CPU or GPU method
void Terrain::computeNormals() {
  if (useCPUOnly) {
    calculateNormalsCPU();
  } else {
    // Use GPU compute shader
    glUseProgram(computeProgram);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vertexBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, indexBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, normalBuffer);

    glUniform1i(glGetUniformLocation(computeProgram, "gridSize"), gridSize);

    glDispatchCompute((gridSize * gridSize + 255) / 256, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
  }
}

// Calculate normals on the CPU
void Terrain::calculateNormalsCPU() {
  normals.clear();
  normals.resize(vertices.size(), 0.0f);

  // Calculate normals for each triangle
  for (size_t i = 0; i < indices.size(); i += 3) {
    unsigned int i0 = indices[i] * 3;
    unsigned int i1 = indices[i + 1] * 3;
    unsigned int i2 = indices[i + 2] * 3;

    glm::vec3 v1(vertices[i0], vertices[i0 + 1], vertices[i0 + 2]);
    glm::vec3 v2(vertices[i1], vertices[i1 + 1], vertices[i1 + 2]);
    glm::vec3 v3(vertices[i2], vertices[i2 + 1], vertices[i2 + 2]);

    glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

    // Accumulate normals for each vertex
    for (int j = 0; j < 3; ++j) {
      normals[i0 + j] += normal[j];
      normals[i1 + j] += normal[j];
      normals[i2 + j] += normal[j];
    }
  }

  // Normalize the accumulated normals
  for (size_t i = 0; i < normals.size(); i += 3) {
    glm::vec3 n(normals[i], normals[i + 1], normals[i + 2]);
    n = glm::normalize(n);
    normals[i] = n.x;
    normals[i + 1] = n.y;
    normals[i + 2] = n.z;
  }

  // Update normal buffer
  glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
  glBufferSubData(GL_ARRAY_BUFFER, 0, normals.size() * sizeof(float),
                  normals.data());
}

// Render normal vectors for visualization
void Terrain::renderNormals() const {
  glDisable(GL_LIGHTING);
  glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for normals
  glBegin(GL_LINES);
  for (size_t i = 0; i < indices.size(); i += 3) {
    glm::vec3 v1(vertices[indices[i] * 3], vertices[indices[i] * 3 + 1],
                 vertices[indices[i] * 3 + 2]);
    glm::vec3 v2(vertices[indices[i + 1] * 3], vertices[indices[i + 1] * 3 + 1],
                 vertices[indices[i + 1] * 3 + 2]);
    glm::vec3 v3(vertices[indices[i + 2] * 3], vertices[indices[i + 2] * 3 + 1],
                 vertices[indices[i + 2] * 3 + 2]);

    glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
    glm::vec3 center = (v1 + v2 + v3) / 3.0f;

    // Draw normal vector
    glVertex3f(center.x, center.y, center.z);
    glVertex3f(center.x + normal.x, center.y + normal.y, center.z + normal.z);
  }
  glEnd();
  glEnable(GL_LIGHTING);
}

// Load shader source code from file
std::string Terrain::loadShaderSource(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to open shader file: " << filename << std::endl;
    return "";
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}