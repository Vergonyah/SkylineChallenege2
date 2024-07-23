#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include "camera.h"

extern bool leftMouseButtonPressed;
extern float lastX;
extern float lastY;
extern bool firstMouse;

void processInput(GLFWwindow* window, Camera& camera, float deltaTime, bool& wireframe, bool& wireframeKeyPressed, bool& showNormals);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

#endif // INPUT_H