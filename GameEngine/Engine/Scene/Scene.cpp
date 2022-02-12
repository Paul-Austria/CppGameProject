#include "Scene.hpp"
#include <spdlog/spdlog.h>
#include <Engine/Entities/Entity.hpp>

#include <Engine/Entities/BaseComponents.hpp>

#include "Engine/Renderer/ImGUI/imgui/imgui.h"
#include "Engine/Renderer/ImGUI/imgui/imgui_internal.h"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/ImGUI/ImGUIStyling.hpp"
namespace GameEngine {
	Scene::Scene()
	{
		registry = entt::registry();
	}
	Scene::~Scene()
	{
		registry.clear();
	}
	void Scene::BackgroundUpdate(float deltaTime,bool isRunning)
	{

		auto renderer = Renderer::GetInstance();

		renderer->BeginRender(editorCam);
		

		auto renderData = registry.view<Renderable>();
		for (auto ent : renderData)
		{
			Renderable renderable = registry.get<Renderable>(ent);
			auto tra = registry.get<TransformComponent>(ent);


			renderer->RenderQuad(renderable, tra, editorCam);
		}

		renderer->EndRender();
	}

	void Scene::EditorUpdate(float deltaTime) {
		Renderer::GetInstance()->StartImGUI();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New"))
				{
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Settings"))
			{
				if (ImGui::BeginMenu("Themes"))
				{
					if (ImGui::MenuItem("LightTheme"))
					{
						ImGUIStyling::LightTheme();
					}
					if (ImGui::MenuItem("DarkTheme"))
					{
						ImGUIStyling::BasicStyle();
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Build"))
			{
				ImGui::EndMenu();
			}
			if(ImGui::BeginMenu("Debug"))
			{
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		} 



		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags =   ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y+18));
		ImGui::SetNextWindowSize(ImVec2(viewport->Size.x,viewport->Size.y-18));
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;


		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", nullptr, window_flags);
		ImGui::PopStyleVar(3);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

			static auto first_time = true;
			if (first_time)
			{
				first_time = false;

				ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
				ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

				// split the dockspace into 2 nodes -- DockBuilderSplitNode takes in the following args in the following order
				//   window ID to split, direction, fraction (between 0 and 1), the final two setting let's us choose which id we want (which ever one we DON'T set as NULL, will be returned by the function)
				//                                                              out_id_at_dir is the id of the node in the direction we specified earlier, out_id_at_opposite_dir is in the opposite direction
				auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);
				auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.25f, nullptr, &dockspace_id);
				auto dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.25f, nullptr, &dockspace_id);

				ImGuiDockNode* node = ImGui::DockBuilderGetCentralNode(dockspace_id);
				// we now dock our windows into the docking node we made above
				ImGui::DockBuilderDockWindow("Down", dock_id_down);
				ImGui::DockBuilderDockWindow("Left", dock_id_left);
				ImGui::DockBuilderDockWindow("Center", node->ID);
				ImGui::DockBuilderDockWindow("Right", dock_id_right);

				ImGui::DockBuilderFinish(dockspace_id);
			}
		}

		ImGui::End();


		ImGui::Begin("Down");
		ImGui::Text("Hello, down!");
		ImGui::End();


		ImGui::Begin("Left");
		ImGui::Text("Hello, left!");

		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = 150;

		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);

		ImGui::Button("TestButton", ImVec2(150, 50));
		ImGui::End();


		ImGui::Begin("Center");
		ImGui::Text("Hello, Center 1");
		ImGui::End();

		ImGui::Begin("Right");
		ImGui::Text("Hello, Center 2");
		ImGui::End();
		Renderer::GetInstance()->EndImGUI();


		

	}


	Entity Scene::CreateEntity(std::string name)
	{

		Entity entity = { registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}


	template<typename T>
	inline void Scene::OnComponentAdded(Entity entity, T& component)
	{
	}

	void Scene::DestroyEntity(Entity ent)
	{
		registry.destroy(ent);
	}

}