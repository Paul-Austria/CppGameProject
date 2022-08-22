#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>

namespace GameEngine
{
    class Window
    {
    public:
        ~Window();

        int GetWidth();
        int GetHeigth();

        void SetWidth(int width);
        void SetHeight(int height);

        GLFWwindow* GetWindow();

        int Init(int width, int height, const char* title, bool fullscreen);

        static Window* GetInstance();

        GLFWwindow* GetHandle() { return windowHandle; }

    private:
        static Window* instance;
        int width = 0, height = 0;
        Window();
        GLFWwindow* windowHandle;

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    };

}
