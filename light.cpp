// light.cpp
// Implements the renderCube function for visualizing light sources

#include "light.h"
#include <GL/glew.h>

void renderCube()
{
    // Render a small cube to represent the light source
    glBegin(GL_QUADS);
    // Front face
    glVertex3f(-0.1f, -0.1f,  0.1f);
    glVertex3f( 0.1f, -0.1f,  0.1f);
    glVertex3f( 0.1f,  0.1f,  0.1f);
    glVertex3f(-0.1f,  0.1f,  0.1f);
    // Back face
    glVertex3f(-0.1f, -0.1f, -0.1f);
    glVertex3f(-0.1f,  0.1f, -0.1f);
    glVertex3f( 0.1f,  0.1f, -0.1f);
    glVertex3f( 0.1f, -0.1f, -0.1f);
    // Top face
    glVertex3f(-0.1f,  0.1f, -0.1f);
    glVertex3f(-0.1f,  0.1f,  0.1f);
    glVertex3f( 0.1f,  0.1f,  0.1f);
    glVertex3f( 0.1f,  0.1f, -0.1f);
    // Bottom face
    glVertex3f(-0.1f, -0.1f, -0.1f);
    glVertex3f( 0.1f, -0.1f, -0.1f);
    glVertex3f( 0.1f, -0.1f,  0.1f);
    glVertex3f(-0.1f, -0.1f,  0.1f);
    // Right face
    glVertex3f( 0.1f, -0.1f, -0.1f);
    glVertex3f( 0.1f,  0.1f, -0.1f);
    glVertex3f( 0.1f,  0.1f,  0.1f);
    glVertex3f( 0.1f, -0.1f,  0.1f);
    // Left face
    glVertex3f(-0.1f, -0.1f, -0.1f);
    glVertex3f(-0.1f, -0.1f,  0.1f);
    glVertex3f(-0.1f,  0.1f,  0.1f);
    glVertex3f(-0.1f,  0.1f, -0.1f);
    glEnd();
}