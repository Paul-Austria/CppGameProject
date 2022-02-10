#include "Renderer.hpp"

#include <spdlog/spdlog.h>
#include <GL/glew.h>
#include "Engine/Renderer/ImGUI/imgui/imgui.h"
#include "Engine/Renderer/ImGUI/imgui/backends/imgui_impl_glfw.h"
#include "Engine/Renderer/ImGUI/imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "Engine/Core/Window.hpp"
#include <filesystem>


#include "ImGUI/ImGUIStyling.hpp"

GameEngine::Renderer* GameEngine::Renderer::instance = 0;
namespace GameEngine {
	Renderer::Renderer()
	{
	}
	Renderer::~Renderer()
	{
	}
	Renderer* Renderer::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new Renderer();
		}
		return instance;
	}
	void Renderer::Init()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		// Setup Dear ImGui style
		ImGUIStyling::BasicStyle();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(Window::GetInstance()->GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 130");

	}
	void Renderer::StartImGUI()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

	}
	void Renderer::EndImGUI()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	}
}