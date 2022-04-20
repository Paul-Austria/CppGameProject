#pragma once
#include <memory>
#include <string>
#include <Engine/Scene/ProjectData.hpp>
#include <memory>

namespace GameEngine {
	class Scene;
	class EditorView;
	class Engine
	{
	public:
		Engine();
		~Engine();

		void Init(int widht, int height, const std::string& title,const std::string &ProjectPath = "", bool isFullScreen = false);
		void Loop();
		
		void SetCurrentScene(std::shared_ptr<Scene> Scene);
		std::shared_ptr<Scene> GetCurrentScene() { return currentScene; }
 		
		static GameEngine::Engine* GetInstance();

		void Reset();


		void LoadProject(const std::string& path);

		ProjectData& GetCurrentProject() { return currentProject; }

	private:

		friend class EditorView;
		friend class ContentBrowser;

		void DevelopUpdate(float deltaTime);
		void SceneUpdate(float deltaTime);

		static std::unique_ptr<GameEngine::Engine>  instance;
		

		ProjectData currentProject;

		std::shared_ptr<Scene> currentScene;
		float LastTime = 0;
		bool isRunning = true;
		bool projectLoaded = false;

	};
}