// input.cpp
// Implements input handling functions

#include "input.h"
#include "light.h"
#include <vector>
#include <iostream>

// Global input state variables
bool leftMouseButtonPressed = false;
float lastX = 400, lastY = 300;
bool firstMouse = true;

// External light vector (defined in main.cpp)
extern std::vector<Light> lights;

void processInput(GLFWwindow* window, Camera& camera, float deltaTime, bool& wireframe, bool& wireframeKeyPressed, bool& showNormals)
{
    // Check for exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Camera movement
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(cameraSpeed, true, false, false, false);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(cameraSpeed, false, true, false, false);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(cameraSpeed, false, false, true, false);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(cameraSpeed, false, false, false, true);

    // Toggle wireframe mode
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        if (!wireframeKeyPressed)
        {
            wireframe = !wireframe;
            glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
            wireframeKeyPressed = true;
        }
    }
    else
    {
        wireframeKeyPressed = false;
    }

    // Toggle normal visualization
    static bool normalKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        if (!normalKeyPressed)
        {
            showNormals = !showNormals;
            normalKeyPressed = true;
        }
    }
    else
    {
        normalKeyPressed = false;
    }

    // Place new light at camera position
    static bool lightKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        if (!lightKeyPressed)
        {
            lights.emplace_back(camera.Position, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
            std::cout << "Light placed at: " << camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z << std::endl;
            lightKeyPressed = true;
        }
    }
    else
    {
        lightKeyPressed = false;
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (leftMouseButtonPressed)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
        camera->ProcessMouseMovement(xoffset, yoffset);
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        leftMouseButtonPressed = true;
        firstMouse = true; 
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        leftMouseButtonPressed = false;
    }
}