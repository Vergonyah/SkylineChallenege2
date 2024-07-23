#include "terrain.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Terrain::Terrain(int gridSize)
    : showNormals(false), 
      gridSize(gridSize),
      heightmapWidth(0), 
      heightmapHeight(0), 
      computeProgram(0),
      heightMapTexture(0),
      normalMapTexture(0) {}


Terrain::~Terrain() {
    glDeleteProgram(computeProgram);
    glDeleteTextures(1, &heightMapTexture);
    glDeleteTextures(1, &normalMapTexture);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indexBuffer);
    glDeleteBuffers(1, &normalBuffer);
}

void Terrain::setShowNormals(bool show) {
    showNormals = show;
}


int Terrain::getTriangleCount() const {
    return indices.size() / 3;
}

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
    calculateNormals();
    setupBuffers();

}

void Terrain::setupBuffers() {
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
}


void Terrain::calculateNormals() {
    normals.clear();
    normals.resize(vertices.size(), 0.0f);

    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned int i0 = indices[i] * 3;
        unsigned int i1 = indices[i+1] * 3;
        unsigned int i2 = indices[i+2] * 3;

        glm::vec3 v1(vertices[i0], vertices[i0+1], vertices[i0+2]);
        glm::vec3 v2(vertices[i1], vertices[i1+1], vertices[i1+2]);
        glm::vec3 v3(vertices[i2], vertices[i2+1], vertices[i2+2]);

        glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

        for (int j = 0; j < 3; ++j) {
            normals[i0+j] += normal[j];
            normals[i1+j] += normal[j];
            normals[i2+j] += normal[j];
        }
    }

    for (size_t i = 0; i < normals.size(); i += 3) {
        glm::vec3 n(normals[i], normals[i+1], normals[i+2]);
        n = glm::normalize(n);
        normals[i] = n.x;
        normals[i+1] = n.y;
        normals[i+2] = n.z;
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
    glEnable(GL_LIGHTING);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexPointer(3, GL_FLOAT, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glNormalPointer(GL_FLOAT, 0, nullptr);

    std::vector<glm::vec3> colors(vertices.size() / 3);
    for (size_t i = 0; i < vertices.size(); i += 3) {
        colors[i / 3] = calculateColor(vertices[i + 1]); 
    }
    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
    glColorPointer(3, GL_FLOAT, 0, nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glDeleteBuffers(1, &colorBuffer);

    glDisable(GL_LIGHTING);

    if (showNormals) {
        renderNormals();
    }
}


void Terrain::initComputeShader() {
    GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
    std::string computeShaderSource = loadShaderSource("compute_shader.glsl");
    const char* source = computeShaderSource.c_str();
    glShaderSource(computeShader, 1, &source, nullptr);
    glCompileShader(computeShader);

    computeProgram = glCreateProgram();
    glAttachShader(computeProgram, computeShader);
    glLinkProgram(computeProgram);

    glDeleteShader(computeShader);

    glGenTextures(1, &heightMapTexture);
    glBindTexture(GL_TEXTURE_2D, heightMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, gridSize, gridSize, 0, GL_RED, GL_FLOAT, vertices.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenTextures(1, &normalMapTexture);
    glBindTexture(GL_TEXTURE_2D, normalMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, gridSize, gridSize, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Terrain::computeNormals() {
    glUseProgram(computeProgram);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vertexBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, indexBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, normalBuffer);

    glUniform1i(glGetUniformLocation(computeProgram, "gridSize"), gridSize);

    glDispatchCompute((gridSize * gridSize + 255) / 256, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void Terrain::renderNormals() const {
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.0f);  // Yellow color for normals
    glBegin(GL_LINES);
    for (size_t i = 0; i < indices.size(); i += 3) {
        glm::vec3 v1(vertices[indices[i] * 3], vertices[indices[i] * 3 + 1], vertices[indices[i] * 3 + 2]);
        glm::vec3 v2(vertices[indices[i+1] * 3], vertices[indices[i+1] * 3 + 1], vertices[indices[i+1] * 3 + 2]);
        glm::vec3 v3(vertices[indices[i+2] * 3], vertices[indices[i+2] * 3 + 1], vertices[indices[i+2] * 3 + 2]);

        glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
        glm::vec3 center = (v1 + v2 + v3) / 3.0f;

        glVertex3f(center.x, center.y, center.z);
        glVertex3f(center.x + normal.x, center.y + normal.y, center.z + normal.z);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

std::string Terrain::loadShaderSource(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filename << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}