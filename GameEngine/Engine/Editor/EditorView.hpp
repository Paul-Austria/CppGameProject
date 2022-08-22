#pragma once
#include <entt/entt.hpp>
#include "ContentBrowser.hpp"

namespace GameEngine {
	class Scene;

	class EditorView
	{
	public:
		EditorView(Scene* currentScene);
		EditorView() = default;
		~EditorView() = default;


		void EditorUpdate(float deltaTime);


		void CreateProject();
		void SaveProject();
		void OpenProject();
	private:
		friend class ContentBrowser;

		Scene* scene;
		bool projectLoaded = false;
		bool newSceneLoaded = false;
		bool showPerformanceWindow = false;
		ContentBrowser browser;

		entt::entity currentEntity;
		bool entitySelected = false;
	};
}