#include "light.h"
#include <GL/glew.h>

void renderCube()
{
    glBegin(GL_QUADS);
    // Front
    glVertex3f(-0.1f, -0.1f,  0.1f);
    glVertex3f( 0.1f, -0.1f,  0.1f);
    glVertex3f( 0.1f,  0.1f,  0.1f);
    glVertex3f(-0.1f,  0.1f,  0.1f);
    // Back 
    glVertex3f(-0.1f, -0.1f, -0.1f);
    glVertex3f(-0.1f,  0.1f, -0.1f);
    glVertex3f( 0.1f,  0.1f, -0.1f);
    glVertex3f( 0.1f, -0.1f, -0.1f);
    // Top 
    glVertex3f(-0.1f,  0.1f, -0.1f);
    glVertex3f(-0.1f,  0.1f,  0.1f);
    glVertex3f( 0.1f,  0.1f,  0.1f);
    glVertex3f( 0.1f,  0.1f, -0.1f);
    // Bottom 
    glVertex3f(-0.1f, -0.1f, -0.1f);
    glVertex3f( 0.1f, -0.1f, -0.1f);
    glVertex3f( 0.1f, -0.1f,  0.1f);
    glVertex3f(-0.1f, -0.1f,  0.1f);
    // Right 
    glVertex3f( 0.1f, -0.1f, -0.1f);
    glVertex3f( 0.1f,  0.1f, -0.1f);
    glVertex3f( 0.1f,  0.1f,  0.1f);
    glVertex3f( 0.1f, -0.1f,  0.1f);
    // Left 
    glVertex3f(-0.1f, -0.1f, -0.1f);
    glVertex3f(-0.1f, -0.1f,  0.1f);
    glVertex3f(-0.1f,  0.1f,  0.1f);
    glVertex3f(-0.1f,  0.1f, -0.1f);
    glEnd();
}