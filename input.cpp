#include "input.h"
#include <iostream>

bool leftMouseButtonPressed = false;
float lastX = 400, lastY = 300;
bool firstMouse = true;

void processInput(GLFWwindow* window, Camera& camera, float deltaTime, bool& wireframe, bool& wireframeKeyPressed)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(cameraSpeed, true, false, false, false);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(cameraSpeed, false, true, false, false);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(cameraSpeed, false, false, true, false);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(cameraSpeed, false, false, false, true);

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
