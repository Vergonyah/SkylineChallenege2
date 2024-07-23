// input.h
// Declares functions and variables for handling user input

#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include "camera.h"

// Global input state variables
extern bool leftMouseButtonPressed;
extern float lastX;
extern float lastY;
extern bool firstMouse;

// Function to process keyboard input
void processInput(GLFWwindow* window, Camera& camera, float deltaTime, bool& wireframe, bool& wireframeKeyPressed, bool& showNormals);

// Callback function for mouse movement
void mouseCallback(GLFWwindow* window, double xpos, double ypos);

// Callback function for mouse button events
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

#endif // INPUT_H