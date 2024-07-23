// window.h
// Defines the Window class for managing the GLFW window

#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    // Public methods
    bool init();
    bool shouldClose() const;
    void clear() const;
    void swapBuffers() const;
    void pollEvents() const;
    GLFWwindow* getWindow() const;

private:
    int width;
    int height;
    std::string title;
    GLFWwindow* window;

    // Callback for window resize
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};

#endif