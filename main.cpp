#include "window.h"
#include "input.h"
#include "terrain.h"
#include "camera.h"
#include "light.h"
#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>

float sensitivity = 0.1f;
Camera camera(glm::vec3(0.0f, 20.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, sensitivity);

bool wireframe = false;
bool wireframeKeyPressed = false;
bool showNormals = false;
std::vector<Light> lights;

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Need to make light cpp and move this into it later. 
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

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <heightmap_path>" << std::endl;
        return -1;
    }
    std::string heightmapPath = argv[1];

    Window window(800, 600, "Terrain Renderer");
    if (!window.init())
    {
        std::cout << "Failed to initialize window" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window.getWindow(), framebufferSizeCallback);
    glfwSetCursorPosCallback(window.getWindow(), mouseCallback);
    glfwSetMouseButtonCallback(window.getWindow(), mouseButtonCallback);
    glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window.getWindow(), &camera);

    Terrain terrain(200);
    if (!terrain.loadHeightmap(heightmapPath)) {
        std::cerr << "Failed to load heightmap. Exiting." << std::endl;
        return -1;
    }
    
    terrain.generate();
    terrain.initComputeShader();
    terrain.setShowNormals(showNormals);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat lightPos[] = {50.0f, 50.0f, 50.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

    int frameCount = 0;
    auto lastTime = std::chrono::high_resolution_clock::now();

    while (!window.shouldClose())
    {
        float currentFrame = glfwGetTime();
        static float lastFrame = 0.0f;
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window.getWindow(), camera, deltaTime, wireframe, wireframeKeyPressed, showNormals);
        terrain.setShowNormals(showNormals);
        
        window.clear();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glLoadMatrixf(glm::value_ptr(projection));

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glm::mat4 view = camera.GetViewMatrix();
        glLoadMatrixf(glm::value_ptr(view));

        for (size_t i = 0; i < lights.size(); ++i) {
            GLenum lightEnum = GL_LIGHT0 + i;
            glEnable(lightEnum);
            GLfloat lightPos[] = {lights[i].position.x, lights[i].position.y, lights[i].position.z, 1.0f};
            glLightfv(lightEnum, GL_POSITION, lightPos);
            GLfloat lightColor[] = {lights[i].color.r, lights[i].color.g, lights[i].color.b, 1.0f};
            glLightfv(lightEnum, GL_DIFFUSE, lightColor);
            glLightf(lightEnum, GL_CONSTANT_ATTENUATION, 1.0f / lights[i].intensity);
        }

        terrain.computeNormals();
        terrain.render();

        glDisable(GL_LIGHTING);
        for (const auto& light : lights) {
            glPushMatrix();
            glTranslatef(light.position.x, light.position.y, light.position.z);
            glColor3f(light.color.r, light.color.g, light.color.b);
            renderCube();
            glPopMatrix();
        }
        glEnable(GL_LIGHTING);

        window.swapBuffers();
        window.pollEvents();

        frameCount++;
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime);

        if (duration.count() >= 1) {
            double fps = frameCount / duration.count();
            std::cout << "FPS: " << fps << std::endl;
            frameCount = 0;
            lastTime = currentTime;
        }
    }

    return 0;
}
