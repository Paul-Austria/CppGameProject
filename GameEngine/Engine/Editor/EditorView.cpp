#include "EditorView.hpp"
#include <Engine/Scene/Scene.hpp>

#include "Engine/Renderer/ImGUI/imgui/imgui.h"
#include "Engine/Renderer/ImGUI/imgui/imgui_internal.h"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/ImGUI/ImGUIStyling.hpp"
#include <Engine/Utils/Profiling/ProfileInstance.hpp>
#include <spdlog/spdlog.h>
#include <Engine/Entities/Entity.hpp>

namespace GameEngine {
	EditorView::EditorView(Scene* currentScene)
	{
		scene = currentScene;
	}

	void EditorView::EditorUpdate(float deltaTime) {
		ProfileInstance::GetInstance()->StartProfileSession("DebugUI");
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
			if (ImGui::BeginMenu("Debug"))
			{
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}



		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + 18));
		ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y - 18));
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
				ImGui::DockBuilderDockWindow("Entities", dock_id_left);
				ImGui::DockBuilderDockWindow("Scene", node->ID);
				ImGui::DockBuilderDockWindow("Entity Info", dock_id_right);

				ImGui::DockBuilderFinish(dockspace_id);
			}
		}

		ImGui::End();


		ImGui::Begin("Down");
		ImGui::Text("Hello, down!");
		ImGui::End();


		ImGui::Begin("Entities");

		auto renderData = scene->registry.view<TagComponent>();

		static ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable | ImGuiTableFlags_ContextMenuInBody;

		ImGui::BeginTable("table2", 1, flags, ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y));

		ImGui::TableSetupColumn("Name");
		ImGui::TableHeadersRow();
		int counter = 0;

		for (auto ent : renderData)
		{
			auto tag = scene->registry.get<TagComponent>(ent);
			ImGui::TableNextRow();

			ImGui::TableNextColumn();
			ImGui::BeginGroup();
			bool selected = false;
			ImGui::Selectable(tag.Tag.c_str(), &selected, 0, ImVec2(ImGui::GetWindowSize().x, 15));
			ImGui::Separator();
			ImGui::EndGroup();
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
				entitySelected = true;
				currentEntity = ent;
			}
			counter++;

		}
		ImGui::EndTable();


		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Create Entity"))
			{
				scene->CreateEntity("New Entity");
			}

			ImGui::EndPopup();
		}


		ImGui::End();

		flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
		bool open = true;
		ImGui::Begin("Scene", &open, flags);

		scene->renderTarget.width = ImGui::GetWindowSize().x;
		scene->renderTarget.height = ImGui::GetWindowSize().y - 20;
		ImGui::Image((void*)scene->renderTarget.ID, ImVec2(scene->renderTarget.width, scene->renderTarget.height));



		if (ImGui::IsItemHovered()) {
			scene->UpdateEditorCam(deltaTime);
		}


		ImGui::End();

		ImGui::Begin("Entity Info");
		if (entitySelected)
		{
			auto string = scene->registry.get<TagComponent>(currentEntity).Tag;

			ImGui::Text(string.c_str());
			ImGui::Separator();
			if (ImGui::CollapsingHeader("Tag")) {
				ImGui::InputText("Name", string.data(), string.size());
				scene->registry.get<TagComponent>(currentEntity).Tag = string;
			}
		
			
			ImGui::Separator();
			if(ImGui::CollapsingHeader("Transformations")) {
				auto transformComponent = scene->registry.get<TransformComponent>(currentEntity);

				ImGui::Text("Position");

				ImGui::DragFloat("X Position", &transformComponent.position.x, 0.01);
				ImGui::DragFloat("Y Position", &transformComponent.position.y, 0.01);
				ImGui::DragFloat("Z Position", &transformComponent.position.z, 0.01);

				ImGui::Text("Scale");

				ImGui::DragFloat("X Scale", &transformComponent.scale.x, 0.01);
				ImGui::DragFloat("Y Scale", &transformComponent.scale.y, 0.01);

				ImGui::Text("Rotation");

				ImGui::DragFloat("Rotation", &transformComponent.rotation, 0.01);

				scene->registry.get<TransformComponent>(currentEntity) = transformComponent;
			}


			if (scene->registry.all_of<Renderable>(currentEntity))
			{
				if (ImGui::CollapsingHeader("Renderable"))
				{
					auto renderable = scene->registry.get<Renderable>(currentEntity);
					bool useColor = renderable.UseColor();
					ImGui::Checkbox("UseColor", &useColor);
					renderable.SetUseColor(useColor);

					int x = renderable.GetWidth();
					int y = renderable.GetHeight();
					ImGui::DragInt("X Render Scale", &x, 0.01);
					ImGui::DragInt("Y Render Scale", &y, 0.01);

					renderable.SetWidth(x);
					renderable.SetHeight(y);


					if (useColor)
					{
						glm::vec4 col = renderable.GetColor();
						float color[] = { col.x,col.y,col.z,col.a };
						ImGui::ColorPicker4("Color", color);
						col.x = color[0];
						col.y = color[1];
						col.z = color[2];
						col.a = color[3];
						renderable.SetColor(col);
					}

					if (renderable.GetVAO() != 0 && !useColor && renderable.GetTexture() != nullptr)
					{
						ImGui::Image((void*)renderable.GetTexture()->ID, ImVec2(ImGui::GetWindowSize().x, 40));
					}
					

					scene->registry.get<Renderable>(currentEntity) = renderable;


					if (ImGui::Button("Delete Renderable", ImVec2(ImGui::GetWindowSize().x, 20)))
					{
						scene->registry.remove<Renderable>(currentEntity);
						
					}
				}

			}


			
			if (ImGui::BeginPopupContextWindow())
			{
				if (!scene->registry.all_of<Renderable>(currentEntity))
				{
					if (ImGui::MenuItem("AddRenderable"))
					{
						scene->registry.emplace<Renderable>(currentEntity,50,50);
					}
				}

				ImGui::EndPopup();
			}


			if (ImGui::Button("Delete", ImVec2(ImGui::GetWindowSize().x, 20)))
			{
				scene->registry.destroy(currentEntity);
				entitySelected = false;
			}

		}
		ImGui::End();
		Renderer::GetInstance()->EndImGUI();

		ProfileInstance::GetInstance()->EndProfileSession("DebugUI");
	}
}