#include "Window.hpp"
#include "spdlog/spdlog.h"

GameEngine::Window* GameEngine::Window::instance = 0;

namespace GameEngine {
	Window::Window() {}

	Window::~Window() {}

	Window* Window::GetInstance()
	{
		if (!instance) {
			spdlog::info("New Window created");
			instance = new Window;
		}
		return instance;
	}

	void Window::SetWidth(int w) { this->width = w; }

	GLFWwindow* Window::GetWindow() { return this->windowHandle; }

	void Window::SetHeight(int h) { this->height = h; }

	int Window::GetHeigth()
	{
		return this->height;
		glViewport(0, 0, this->width, this->height);
	}

	int Window::GetWidth()
	{
		return this->width;
		glViewport(0, 0, this->width, this->height);
	}

	int Window::Init(int width, int height, const char* title, bool fullscreen)
	{
		SetWidth(width);
		SetHeight(height);

		if (!glfwInit())
			return -1;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


		if (!fullscreen) windowHandle = glfwCreateWindow(width, height, title, NULL, NULL);
		else windowHandle = glfwCreateWindow(width, height, title, glfwGetPrimaryMonitor(), NULL);
		if (!windowHandle) {
			glfwTerminate();
			return -1;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(windowHandle);
		glewExperimental = GL_TRUE;
		glfwSetFramebufferSizeCallback(windowHandle, framebuffer_size_callback);

		if (glewInit() != GLEW_OK)
			spdlog::critical("glew not init");

		glViewport(0, 0, width, height);

		return 0;
	}

	void Window::framebuffer_size_callback(GLFWwindow* w, int wi, int h)
	{
		glViewport(0, 0, wi, h);


#ifdef DEBUG
		spdlog::info("New Window size: {0} {1}", wi, h);
#endif // DEBUG


		Window::GetInstance()->SetHeight(h);
		Window::GetInstance()->SetWidth(wi);
	}

}
