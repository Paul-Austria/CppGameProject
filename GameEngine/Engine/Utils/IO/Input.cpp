#include "Input.hpp"
#include <Engine/Core/Window.hpp>
namespace GameEngine {
	bool Input::IsKeyPressed(char key)
	{
		auto inst = Window::GetInstance();
		return glfwGetKey(inst->GetWindow(), key) == GLFW_PRESS;
	}
}