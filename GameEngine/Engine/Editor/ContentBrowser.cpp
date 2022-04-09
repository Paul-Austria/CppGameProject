#include "ContentBrowser.hpp"
#include "EditorView.hpp"
#include <Engine/Engine.hpp>
#include <Engine/Renderer/ImGUI/imgui/imgui.h>
#include <Engine/Utils/StringHelpers.hpp>
#include <vector>
#include <algorithm>

#include <Engine/ResourceManagement/TextureResourceManager.hpp>

#include <spdlog/spdlog.h>
namespace GameEngine
{
	ContentBrowser::ContentBrowser(EditorView* editor) {
		this->editor = editor;
	}


	ContentBrowser::~ContentBrowser() {

	}


	void ContentBrowser::HandleFileClick(std::string name)
	{
		spdlog::info(name);
	}

	void ContentBrowser::Render() {
		auto newPath = Engine::GetInstance()->GetCurrentProject().GetPath();
		
		if (newPath != rootPath)
		{
			rootPath = std::string(newPath);
			currentSubPath = std::string();
		}


		int startY = ImGui::GetCursorPosY();
		auto split = StringHelpers::Split(currentSubPath, "/");
		split.pop_back();
		if (ImGui::Button("Root/", ImVec2(ImGui::CalcTextSize("Root/").x+5, 20)))
		{
			currentSubPath = "";
		}
		ImGui::SetCursorPosY(startY);
		float size = (ImGui::CalcTextSize("Root/").x);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + size+5);
		for (auto var : split)
		{
			
			
			if (ImGui::Button((var+"/").c_str(),ImVec2(ImGui::CalcTextSize((var + "/").c_str()).x+5, 20)))
			{

				

				auto loc = std::distance(split.begin(), std::find(split.begin(), split.end(), var));
				currentSubPath = "";
				for (int i = 0; i < loc; i++)
				{
					currentSubPath += split.at(i) + "/";
				}
			}
			ImGui::SetCursorPosY(startY);
			float size = (ImGui::CalcTextSize((var+"/").c_str()).x);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + size + 10);

		}



		ImGui::SetCursorPosY(startY + 20);

		ImGui::Separator();

		int columnSize = (int)ImGui::GetWindowWidth() / 100;
		ImGui::Columns(columnSize, "Files",false);


		for (const auto& entry : std::filesystem::directory_iterator(rootPath+"/"+currentSubPath)) {
			std::string pa = std::string();
			pa = entry.path().filename().string();



			if (entry.is_directory())
			{
				if (ImGui::Button(pa.c_str(),ImVec2(100,100)))
				{
					
					currentSubPath += pa+"/";
					
				}
				ImGui::NextColumn();
			}
			else {
				auto extension = StringHelpers::GetExtension(pa);
				unsigned int textureID = 0;
				if (extension == "png")
				{
					if (TextureResourceManager::GetInstance()->TextureExists(currentSubPath + pa))
					{
						textureID = TextureResourceManager::GetInstance()->GetTexture(currentSubPath + pa)->ID;
					}
					else
					{
						TextureResourceManager::GetInstance()->LoadTexture(currentSubPath + pa, rootPath + "/" + currentSubPath + pa);
						textureID = TextureResourceManager::GetInstance()->GetTexture(currentSubPath + pa)->ID;
					}
				}
				if (ImGui::ImageButton((void*)textureID, ImVec2(100, 100)))
				{
					HandleFileClick(pa);
				}
				auto windowWidth = ImGui::GetWindowWidth() / columnSize;
				auto textWidth = ImGui::CalcTextSize(pa.c_str()).x;
				ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f + ImGui::GetCursorPosX());
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 15);
				ImGui::Text(pa.c_str());
				ImGui::NextColumn();
			}
		}

		ImGui::Columns();



		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Create_New_File"))
			{
			}

			ImGui::EndPopup();
		}

		
	}



}
