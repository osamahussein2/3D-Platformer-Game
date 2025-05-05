#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

using namespace std;

class Window
{
public:
    ~Window();

    static Window* Instance();

    // Initialize OpenGL window here
    void InitializeWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);

    void UpdateWindow();

private:
    // Initialize the window's variables to be NULL first before we do anything with it
    Window();

    // Make this function static to use it inside the glfwSetFramebufferSizeCallback function

    /* If I don't make this static, it'll give an error that it cannot convert this function for the Window class
    to a parameter type of GLFWframebuffersizefun */
    static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

    static Window* windowInstance;

    GLFWwindow* openGLwindow;
};

#endif