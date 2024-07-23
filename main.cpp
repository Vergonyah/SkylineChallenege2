#include "window.h"
#include "input.h"
#include "terrain.h"
#include "camera.h"
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float sensitivity = 0.1f;
Camera camera(glm::vec3(0.0f, 20.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, sensitivity);

bool wireframe = false;
bool wireframeKeyPressed = false;

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat lightPos[] = {50.0f, 50.0f, 50.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL); // Set initial polygon mode

    while (!window.shouldClose())
    {
        float currentFrame = glfwGetTime();
        static float lastFrame = 0.0f;
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window.getWindow(), camera, deltaTime, wireframe, wireframeKeyPressed);
        window.clear();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glLoadMatrixf(glm::value_ptr(projection));

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glm::mat4 view = camera.GetViewMatrix();
        glLoadMatrixf(glm::value_ptr(view));

        terrain.render();

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}
