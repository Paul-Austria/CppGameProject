#pragma once
#include <entt/entt.hpp>
#include "ContentBrowser.hpp"
#include <memory>
namespace GameEngine {
	class Scene;

	class EditorView
	{
	public:
		EditorView();
		~EditorView() = default;


		void EditorUpdate(float deltaTime);


		void CreateProject();
		void SaveProject();
		void OpenProject();
		void SetCurrentScene(std::shared_ptr<Scene> scene);
	private:
		friend class ContentBrowser;

		std::shared_ptr<Scene> scene;
		bool projectLoaded = false;
		bool newSceneLoaded = false;
		bool projectIsOpen = false;
		bool showPerformanceWindow = false;
		ContentBrowser browser;

		entt::entity currentEntity;
		bool entitySelected = false;
	};
}