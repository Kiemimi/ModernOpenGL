#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace GLFWCallbacks
{
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void processInput(GLFWwindow* window);
}