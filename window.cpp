#include "window.h"

Window::Window(int width, int height, const std::string& title)
    : width(width), height(height), title(title), window(nullptr) {}

Window::~Window()
{
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

bool Window::init()
{
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return false;
    }


    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    return true;
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(window);
}

void Window::clear() const
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
}

void Window::swapBuffers() const
{
    glfwSwapBuffers(window);
}

void Window::pollEvents() const
{
    glfwPollEvents();
}

GLFWwindow* Window::getWindow() const
{
    return window;
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}